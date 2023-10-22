// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Character/MainCharacterAnimInstance.h"
#include "Character/TargetingComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/MainHUD.h"
#include "HUD/MainOverlay.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = 550.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 50.0f;
	CameraBoom->CameraRotationLagSpeed = 50.0f;
	CameraBoom->CameraLagMaxDistance = 10.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("MainCamera"));
	Camera->SetupAttachment(CameraBoom);	

	combatComp = CreateDefaultSubobject<UCombatComponent>(FName("Combat Component"));
	stateManager = CreateDefaultSubobject<UStateManagerComponent>(FName("State Manager Component"));
	targetingComp = CreateDefaultSubobject<UTargetingComponent>(FName("Targeting Component"));
	attributes = CreateDefaultSubobject<UAttributeComponent>(FName("Attributes"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Player"));
	Controller.Get()->Tags.Add("Player");

	animInstance = Cast<UMainCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	InitializeMainOverlay();
}

void AMainCharacter::InitializeMainOverlay()
{
	playerController = Cast<APlayerController>(GetController());
	if (playerController)
	{
		AMainHUD* mainHUD = Cast<AMainHUD>(playerController->GetHUD());
		if (mainHUD)
		{
			mainOverlay = mainHUD->GetOverlay();
			if (mainOverlay && attributes)
			{
				mainOverlay->SetHealthPercentage(attributes->GetHealthPercent());
			}
		}
	}
}

UMainOverlay* AMainCharacter::GetMainOverlay()
{
	return mainOverlay;
}

bool AMainCharacter::IsTargetLocked() const
{
	return targetingComp->IsLocked(); 
}

void AMainCharacter::SetCharacterActionState(ECharacterActions state)
{
	stateManager->SetCharacterActionState(state);
}

ECharacterActions AMainCharacter::GetCharacterActionState()
{
	return stateManager->GetCharacterActionState();
}

USpringArmComponent* AMainCharacter::GetCameraBoom()
{
	return CameraBoom;
}

void AMainCharacter::ResetTargetLock(AActor* _enemy)
{
	targetingComp->TargetedEnemyDied(_enemy);
}

void AMainCharacter::MoveForward(float value)
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	inputZ = value * 2; 

	if (Controller && inputZ != 0)
	{
		const FVector directionVector = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
		AddMovementInput(directionVector, inputZ);		
	}

}

void AMainCharacter::MoveRight(float value)
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	inputX = value * 2;

	if (Controller && inputX != 0)
	{
		const FVector directionVector = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
		AddMovementInput(directionVector, inputX);
	}

}

void AMainCharacter::Turn(float value)
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	if (Controller && value != 0)
	{	
		if(stateManager->GetCharacterRotationState() == ECharacterRotation::Movement)
			AddControllerYawInput(value);
	}
}

void AMainCharacter::LookUp(float value)
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	if (Controller && value != 0)
	{
		AddControllerPitchInput(value);
	}
}

void AMainCharacter::Sprint(float value)
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	if (value > 0.5f && attributes && attributes->HasSprintStamina())
	{
		isSprinting = true;
		attributes->UseStaminaSprint();
	}
	else
	{
		isSprinting = false;
	}
}

void AMainCharacter::JumpPressed()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	if (PlayerCanJump())
	{
		combatComp->ResetWeapon();
		if (jumpSFX)
			UGameplayStatics::PlaySoundAtLocation(this, jumpSFX, GetActorLocation());
		Jump();
	}
}

bool AMainCharacter::PlayerCanJump()
{
	return (!animInstance->IsAnyMontagePlaying() && animInstance->canJump);
}

void AMainCharacter::InteractPressed()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	AWeapon* overlappingWeapon = Cast<AWeapon>(overlappingItem);
	if (overlappingWeapon && !overlappingWeapon->isShield)
	{
		if (combatComp->GetMainWeapon() != nullptr)
		{
			combatComp->GetMainWeapon()->DropWeapon();
		}
		if (combatComp->GetOffHandWeapon() != nullptr)
		{
			combatComp->GetOffHandWeapon()->Destroy();
			combatComp->SetOffHandWeapon(nullptr);
		}
		overlappingWeapon->Equip(this->GetMesh());
		combatComp->SetMainWeapon(overlappingWeapon);

		SetCharacterState(ECharacterState::Equipped);
		//charState = ECharacterState::Equipped;
	}
}

void AMainCharacter::Attack()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	if (GetCharacterState() == ECharacterState::Unequipped) return;
	if (!combatComp->GetMainWeapon()) return;

	if (GetCharacterActionState() == ECharacterActions::None)
	{
		combatComp->PerformAttack();
	}
}

void AMainCharacter::AttackToggle()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	if (!combatComp->GetMainWeapon()) return;

	if (GetCharacterState() == ECharacterState::Unequipped)
	{
		if (animInstance)
		{
			animInstance->Montage_Play(combatComp->GetMainWeapon()->swordDrawMontage);
			SetCharacterState(ECharacterState::Equipped);
		}
	}
	else if (GetCharacterState() == ECharacterState::Equipped)
	{
		if (animInstance)
		{
			animInstance->Montage_Play(combatComp->GetMainWeapon()->swordDisarmMontage);
			SetCharacterState(ECharacterState::Unequipped);
		}
	}

}

void AMainCharacter::Dodge()
{
	if (GetCharacterState() == ECharacterState::Dead) return;
	if (attributes && attributes->HasDodgeStamina())
	{
		combatComp->PerformDodge();
	}
}

void AMainCharacter::LockTarget()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	if(GetCharacterState() == ECharacterState::Equipped)
		targetingComp->LockTarget();
}


void AMainCharacter::EquipMainWeapon()
{
	if (combatComp->GetMainWeapon())
	{
		combatComp->GetMainWeapon()->Equip(this->GetMesh());
	}

	if (combatComp->GetOffHandWeapon())
	{
		combatComp->GetOffHandWeapon()->Equip(this->GetMesh());
	}
}

void AMainCharacter::UnEquipMainWeapon()
{
	if (combatComp->GetMainWeapon())
	{
		combatComp->GetMainWeapon()->UnEquip(this->GetMesh());
	}

	if (combatComp->GetOffHandWeapon())
	{
		combatComp->GetOffHandWeapon()->UnEquip(this->GetMesh());
	}
}

void AMainCharacter::Block()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	isBlockKeyDown = true;
	if (GetCharacterState() == ECharacterState::Unequipped) return;
	if (!combatComp->GetMainWeapon()) return;

	if (GetCharacterActionState() == ECharacterActions::None)
	{
		SetCharacterActionState(ECharacterActions::Blocking);
	}
}

void AMainCharacter::UnBlock()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	isBlockKeyDown = false;
	SetCharacterActionState(ECharacterActions::None);
}

void AMainCharacter::Heal()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	if (attributes && attributes->CanHeal() && GetCharacterActionState() == ECharacterActions::None &&
		!(GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsFlying()))
	{
		if (animInstance && healMontage)
		{
			animInstance->Montage_Play(healMontage);		

			if (mainOverlay)
			{
				mainOverlay->SetHealthPercentage(attributes->GetHealthPercent());
			}

			SetCharacterActionState(ECharacterActions::Healing);
		}
	}
}

void AMainCharacter::HealPlayer()
{
	if (GetCharacterState() == ECharacterState::Dead) return;

	attributes->Heal(50.0f);
}

void AMainCharacter::SetDirection()
{
	FVector v = FVector(inputZ, -inputX, 0);
	FRotator rotB = FRotationMatrix::MakeFromX(v).Rotator();
	FRotator camRot = GetControlRotation();
	FRotator rotA = UKismetMathLibrary::NormalizedDeltaRotator(camRot, RootComponent->GetComponentRotation());
	direction = (UKismetMathLibrary::NormalizedDeltaRotator(rotA, rotB)).Yaw;

	float _speed = FMath::Clamp((FMath::Abs(inputX) + FMath::Abs(inputZ)), 0.0f, 1.0f);
	if (isSprinting)
	{
		_speed *= 2;
	}

	if (_speed > 0.01f)
	{
		rotspeed += (GetWorld()->DeltaTimeSeconds * 200);
	}
	else
	{
		rotspeed -= (GetWorld()->DeltaTimeSeconds * 200);
	}
	rotspeed = FMath::Clamp(rotspeed, 180.0f, 360.0f);

	if (stateManager->GetCharacterRotationState() == ECharacterRotation::Camera)
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	}
	else
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, rotspeed, 0.0f);
	}	
}




// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterState() == ECharacterState::Dead) return;

	SetDirection();	

	if (attributes)
	{
		attributes->RegenStamina(DeltaTime);
		if (mainOverlay)
		{
			mainOverlay->SetStaminaPercentage(attributes->GetStaminaPercent());
		}
	}

	if (!isAlive() && !isDead)
	{
		Die();
	}

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Action State = %d"), stateManager->GetCharacterActionState()));
	}*/
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("Sprint"), this, &AMainCharacter::Sprint);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AMainCharacter::JumpPressed);
	PlayerInputComponent->BindAction(FName("Interact"), IE_Pressed, this, &AMainCharacter::InteractPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction(FName("AttackToggle"), IE_Pressed, this, &AMainCharacter::AttackToggle);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AMainCharacter::Dodge);
	PlayerInputComponent->BindAction(FName("LockTarget"), IE_Pressed, this, &AMainCharacter::LockTarget);
	PlayerInputComponent->BindAction(FName("Block"), IE_Pressed, this, &AMainCharacter::Block);
	PlayerInputComponent->BindAction(FName("Block"), IE_Released, this, &AMainCharacter::UnBlock);
	PlayerInputComponent->BindAction(FName("Heal"), IE_Released, this, &AMainCharacter::Heal);
}

void AMainCharacter::GetHit(const FVector& impactPoint)
{
	if (stateManager->GetCharacterActionState() == ECharacterActions::Dodging) return;
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Player Got Hit")));*/

	if (isAlive())
	{
		DirectionalHitReact(impactPoint);
	}
	else
	{
		Die();
	}
}

void AMainCharacter::DirectionalHitReact(const FVector& impactPoint)
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

	if ((GetCharacterActionState() == ECharacterActions::Blocking) && section == "FromFront")
	{
		if(animInstance)
			animInstance->Montage_Play(combatComp->GetMainWeapon()->blockHitMontage);
	}
	else
	{
		PlayHitReaction(section);
	}
}

void AMainCharacter::PlayHitReaction(const FName sectionName)
{
	if (GetMesh()->GetAnimInstance() && hitReactionMontages)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(hitReactionMontages);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(sectionName);
		stateManager->SetCharacterActionState(ECharacterActions::GotHit);
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (attributes)
	{
		attributes->ReceiveDamage(DamageAmount);

		if (mainOverlay)
		{
			mainOverlay->SetHealthPercentage(attributes->GetHealthPercent());
		}
	}
	return 0.0f;
}


bool AMainCharacter::isAlive()
{
	return attributes->isAlive();
}

void AMainCharacter::Die()
{
	isDead = true;

	SetCharacterState(ECharacterState::Dead);

	if (animInstance)
	{
		animInstance->Die();
	}


	if (mainOverlay)
	{
		mainOverlay->ShowYouDied();

		playerController->bShowMouseCursor = true;
		playerController->bEnableClickEvents = true;
		playerController->bEnableMouseOverEvents = true;
	}
}