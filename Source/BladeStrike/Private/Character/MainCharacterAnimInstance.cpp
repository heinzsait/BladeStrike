// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacterAnimInstance.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

		isInAir = movementComp->IsFalling();
		isDodging = character->isDodging;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Purple, FString::Printf(TEXT("dodging = %d"), isDodging));

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

	}
}
