// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/AI/EnemyAIController.h"
#include "Character/MainCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Perception/AISense_Damage.h"
#include "MotionWarpingComponent.h"
#include "HUD/MainOverlay.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	attributes = CreateDefaultSubobject<UAttributeComponent>(FName("Attributes"));
	healthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(FName("HealthBar"));
	healthBarWidget->SetupAttachment(GetRootComponent());

	motionWrapComp = CreateDefaultSubobject<UMotionWarpingComponent>(FName("Motion Wrapping"));
}

bool AEnemy::isAlive()
{
	return attributes->isAlive();
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));
	player = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(this, AMainCharacter::StaticClass()));

	if (mainWeapon)
	{
		mainWeapon->Equip(this->GetMesh());
	}

	if (healthBarWidget)
	{
		healthBarWidget->SetHealthPercentage(1.0f);
		healthBarWidget->SetVisibility(false);
	}

	if (isBoss)
	{
		if (player && player->GetMainOverlay())
		{
			player->GetMainOverlay()->SetBossHealthPercentage(1.0f);
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMotionWrapping();
}

void AEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (mainWeapon)
	{
		mainWeapon->DropWeapon();
		if (offHandWeapon != nullptr)
		{
			offHandWeapon->Destroy();
			offHandWeapon = nullptr;
		}
	}
}

void AEnemy::RotateTowardsPlayer()
{
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation());
	targetRot = FRotator(GetActorRotation().Pitch, targetRot.Yaw, GetActorRotation().Roll);
	auto lerpRot = FMath::RInterpTo(GetActorRotation(), targetRot, GetWorld()->DeltaTimeSeconds, 360);
	SetActorRotation(lerpRot);
}

void AEnemy::GetHit(const FVector& impactPoint)
{
	if (actionState == ECharacterActions::Dodging) return;

	hitImpactPoint = impactPoint;
	if (isAlive())
	{
		DirectionalHitReact(impactPoint);
	}
	else
	{
		Die();
		HideHealthBar();
	}
}

void AEnemy::ShowHealthBar()
{
	if (!isBoss)
	{
		if (healthBarWidget)
		{
			healthBarWidget->SetVisibility(true);
		}
	}
	else
	{
		if (player && player->GetMainOverlay())
		{
			player->GetMainOverlay()->ShowBossHealth();
		}
	}
}

void AEnemy::HideHealthBar()
{
	if (!isBoss)
	{
		if (healthBarWidget)
		{
			healthBarWidget->SetVisibility(false);
		}
	}
	else
	{
		if (player && player->GetMainOverlay())
		{
			player->GetMainOverlay()->HideBossHealth();
		}
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (attributes && healthBarWidget)
	{
		attributes->ReceiveDamage(DamageAmount);
		if (!isBoss)
		{
			healthBarWidget->SetHealthPercentage(attributes->GetHealthPercent());
		}
		else
		{
			if (player && player->GetMainOverlay())
			{
				player->GetMainOverlay()->SetBossHealthPercentage(attributes->GetHealthPercent());
			}
		}
	}

	UAISense_Damage::ReportDamageEvent(this, this, EventInstigator->GetPawn(), DamageAmount, EventInstigator->GetPawn()->GetActorLocation(), hitImpactPoint);

	return 0.0f;
}

void AEnemy::DirectionalHitReact(const FVector& impactPoint)
{
	const FVector forward = GetActorForwardVector();
	const FVector _impactPoint = FVector(impactPoint.X, impactPoint.Y, GetActorLocation().Z);
	const FVector toHit = (_impactPoint - GetActorLocation()).GetSafeNormal();

	const double cosO = FVector::DotProduct(forward, toHit);
	double O = FMath::RadiansToDegrees(FMath::Acos(cosO));
	if (FVector::CrossProduct(forward, toHit).Z < 0)  O *= -1;

	FName section = FName("FromBack");
	if (O >= -45.f && O < 45.f)  section = FName("FromFront");
	else if (O >= -135.f && O < -45.f)  section = FName("FromLeft");
	else if (O >= 45.f && O < 135.f)  section = FName("FromRight");

	PlayHitReaction(section);
}

void AEnemy::PlayHitReaction(const FName sectionName)
{
	if (GetMesh()->GetAnimInstance() && hitReactionMontages)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(hitReactionMontages);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(sectionName);
	}
}

void AEnemy::Die()
{
	if (GetMesh()->GetAnimInstance())
	{
		UEnemyAnimInstance* _anim = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
		if (_anim)
		{
			_anim->StopAllMontages(0.1f);
			_anim->Die();
		}
	}

	AEnemyAIController* aiC = GetController<AEnemyAIController>();
	if (aiC)
	{
		aiC->Die();
	}	

	if (player)
	{
		player->ResetTargetLock(Cast<AActor>(this));
	}

	currentState = ECharacterState::Dead;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.0f);
}

float AEnemy::PerformAction(EAIAttackType _attackType)
{
	float duration = 0.0f;

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Perform Action : %d"), actionState));

	switch (actionState)
	{
	case ECharacterActions::None:

		break;

	case ECharacterActions::Attacking:
		duration = PerformAttack(_attackType);
		break;

	case ECharacterActions::Dodging:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString("Perform Dodge..."));

		if (dodgeMontage)
		{
			if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(dodgeMontage))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(dodgeMontage);
			}
		}

		break;

	default:
		break;
	}

	return duration;
}

float AEnemy::PerformAttack(EAIAttackType _attackType)
{
	float duration = 0.0f;
	currentAttackType = _attackType;
	if (GetMesh()->GetAnimInstance())
	{
		if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		{
			UAnimMontage* attackMontageToPlay = nullptr;
			switch (_attackType)
			{

			case EAIAttackType::Default:
				if (mainWeapon)
				{
					int index = FMath::RandRange(0, mainWeapon->comboAttackMontages.Num() - 1);
					if (mainWeapon->comboAttackMontages[index])
						attackMontageToPlay = mainWeapon->comboAttackMontages[index];
				}
				break;

			case EAIAttackType::CloseRange:
				if (closeRangeAttacks.Num() > 0)
				{
					int index = FMath::RandRange(0, closeRangeAttacks.Num() - 1);
					if (closeRangeAttacks[index])
						attackMontageToPlay = closeRangeAttacks[index];
				}
				break;

			case EAIAttackType::MediumRange:
				if (mediumRangeAttacks.Num() > 0)
				{
					int index = FMath::RandRange(0, mediumRangeAttacks.Num() - 1);
					if (mediumRangeAttacks[index])
						attackMontageToPlay = mediumRangeAttacks[index];
				}
				break;

			case EAIAttackType::LongRange:
				if (longRangeAttacks.Num() > 0)
				{
					int index = FMath::RandRange(0, longRangeAttacks.Num() - 1);
					if (longRangeAttacks[index])
						attackMontageToPlay = longRangeAttacks[index];
				}
				break;

			default:
				if (mainWeapon)
				{
					int index = FMath::RandRange(0, mainWeapon->comboAttackMontages.Num() - 1);
					if (mainWeapon->comboAttackMontages[index])
						attackMontageToPlay = mainWeapon->comboAttackMontages[index];
				}
				break;

			}

			if (attackMontageToPlay)
			{
				GetMesh()->GetAnimInstance()->Montage_Play(attackMontageToPlay);
				duration = attackMontageToPlay->GetPlayLength();
			}
		}
		else return - 1;
	}
	return duration;
}

FRotator AEnemy::GetRotationWrapTarget()
{
	if (player)
	{
		return (player->GetActorRotation());
	}
	return FRotator();
}


FVector AEnemy::GetTranslationWrapTarget()
{
	if(!player)
		return FVector();

	return (player->GetActorLocation() + (((GetActorLocation() - player->GetActorLocation()).GetSafeNormal()) * wrapTargetDistance));
}


void AEnemy::UpdateMotionWrapping()
{
	if (!player) return;

	motionWrapComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("WrapTarget"), GetTranslationWrapTarget(), GetRotationWrapTarget());
}