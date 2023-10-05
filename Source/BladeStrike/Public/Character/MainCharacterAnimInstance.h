// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypesh.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class AMainCharacter* character;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* movementComp;

	UPROPERTY(BlueprintReadOnly)
	bool isInAir;

	UPROPERTY(BlueprintReadOnly)
	float speed;

	UPROPERTY(BlueprintReadOnly)
	bool isMoving;

	UPROPERTY(BlueprintReadOnly)
	bool receivedInitialDirection;

	UPROPERTY(BlueprintReadOnly)
	bool isControllerSprinting;

	UPROPERTY(BlueprintReadOnly)
	float direction;

	UPROPERTY(BlueprintReadOnly)
	float inputX;

	UPROPERTY(BlueprintReadOnly)
	float inputZ;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState charState = ECharacterState::Unequipped;

};
