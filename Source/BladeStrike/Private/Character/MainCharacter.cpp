// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Items/Weapons/Weapon.h"

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
	
}



// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	animInstance = GetMesh()->GetAnimInstance();
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMainCharacter::MoveForward(float value)
{
	if (actionState != EActionState::Free) return;
	inputZ = value;

	if (Controller && inputZ != 0)
	{
		const FVector directionVector = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
		AddMovementInput(directionVector, inputZ);		
	}

}



void AMainCharacter::MoveRight(float value)
{
	if (actionState != EActionState::Free) return;
	inputX = value;

	if (Controller && inputX != 0)
	{
		const FVector directionVector = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
		AddMovementInput(directionVector, inputX);
	}

}

void AMainCharacter::Turn(float value)
{
	if (Controller && value != 0)
	{
		AddControllerYawInput(value);
	}
}

void AMainCharacter::LookUp(float value)
{
	if (Controller && value != 0)
	{
		AddControllerPitchInput(value);
	}
}

void AMainCharacter::Sprint(float value)
{
	if (value > 0.5f)
	{
		isSprinting = true;
	}
	else
	{
		isSprinting = false;
	}
}

void AMainCharacter::JumpPressed()
{
	Jump();
}

void AMainCharacter::InteractPressed()
{
	AWeapon* overlappingWeapon = Cast<AWeapon>(overlappingItem);
	if (overlappingWeapon)
	{
		overlappingWeapon->Equip(this->GetMesh(), FName("RightHandSocket"));
		charState = ECharacterState::Equipped;
	}
}

void AMainCharacter::Attack()
{
	if (charState != ECharacterState::Equipped) return;

	if (actionState == EActionState::Free)
	{
		if (animInstance)
		{
			animInstance->Montage_Play(attackMontage);
			actionState = EActionState::Attacking1;
		}
	}
	else if (actionState == EActionState::CanAttack2)
	{
		actionState = EActionState::Attacking2;
	}
	else if (actionState == EActionState::CanAttack3)
	{
		actionState = EActionState::Attacking3;
	}
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

	GetCharacterMovement()->RotationRate = FRotator(0.0f, rotspeed, 0.0f);

	/*
	if (CurveFloatReset)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgressReset"));
		CurveTimelineReset.AddInterpFloat(CurveFloatReset, TimelineProgress);

		startRot = FVector(0, 0, 350);
		endRot = FVector(0, 0, 50);

		CurveTimelineReset.PlayFromStart();
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue, FString::Printf(TEXT("Dir = %f"), direction));
	}	
	*/
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetDirection();	
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
}