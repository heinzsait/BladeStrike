// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacterAnimInstance.h"
#include "Character/MainCharacter.h"
#include "Character/StateManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	character = Cast<AMainCharacter>(TryGetPawnOwner());
	if (character)
	{
		movementComp = character->GetCharacterMovement();
	}
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (character)
	{
		charState = character->GetCharacterState();
		combatType = character->GetCombatState();
		rotationState = character->GetRotationState();

		isInAir = movementComp->IsFalling();
		isDodging = character->isDodging;

		inputX = character->inputX;
		inputZ = character->inputZ;

		speed = FMath::Clamp((FMath::Abs(inputX) + FMath::Abs(inputZ)), 0.0f, 1.0f);
		if (character->isSprinting) speed *= 2;

		if (speed > 0.01f)
		{
			if (receivedInitialDirection)
			{
				receivedInitialDirection = false;
			}
			else
			{
				direction = character->direction;

				//Delay of 0.1sec
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindLambda([&]
					{						
						receivedInitialDirection = true;
					});

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);
			}
		}

		CalculateStrafeDirection();

	}
}

void UMainCharacterAnimInstance::CalculateStrafeDirection()
{
	if (rotationState == ECharacterRotation::Camera)
	{
		direction = CalculateDirection(character->GetCharacterMovement()->Velocity, character->GetActorRotation());
		speed = UKismetMathLibrary::VSizeXY(character->GetCharacterMovement()->Velocity);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Purple, FString::Printf(TEXT("speed = %f direction = %f") , speed , direction));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Purple, FString::Printf(TEXT("not camera")));
	}
}
