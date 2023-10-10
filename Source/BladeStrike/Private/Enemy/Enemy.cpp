// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"
#include "AIController.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"

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

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(this, AMainCharacter::StaticClass()));

	if (mainWeapon)
	{
		mainWeapon->Equip(this->GetMesh());
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::RotateTowardsPlayer()
{
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation());
	auto lerpRot = FMath::RInterpTo(GetActorRotation(), targetRot, GetWorld()->DeltaTimeSeconds, 360);
	SetActorRotation(lerpRot);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& impactPoint)
{
	DirectionalHitReact(impactPoint);
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

float AEnemy::PerformAction()
{
	float duration = 0.0f;

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Perform Action : %d"), actionState));

	switch (actionState)
	{
	case ECharacterActions::None:

		break;

	case ECharacterActions::Attacking:
		duration = PerformAttack();
		break;

	case ECharacterActions::Dodging:
		
		break;

	default:
		break;
	}

	return duration;
}

float AEnemy::PerformAttack()
{
	float duration = 0.0f;
	if (GetMesh()->GetAnimInstance())
	{
		if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		{
			if (mainWeapon)
			{
				int index = FMath::RandRange(0, mainWeapon->attackMontages.Num() - 1);
				if (mainWeapon->attackMontages[index])
				{
					GetMesh()->GetAnimInstance()->Montage_Play(mainWeapon->attackMontages[index]);
					duration = mainWeapon->attackMontages[index]->GetPlayLength();
				}
			}
		}
	}
	return duration;
}



//void AEnemy::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
//{
//
//}

