// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnumClass.h"
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

	UPROPERTY(BlueprintReadWrite)
	bool canJump = true;

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
	bool isDodging;

	UPROPERTY(BlueprintReadOnly)
	bool isBlocking;

	UPROPERTY(BlueprintReadOnly)
	ECharacterState charState = ECharacterState::Unequipped;

	UPROPERTY(BlueprintReadOnly)
	ECombatTypes combatType = ECombatTypes::None;

	UPROPERTY(BlueprintReadOnly)
	ECharacterRotation rotationState = ECharacterRotation::Movement;

	UFUNCTION()
	void Die();

	UPROPERTY(BlueprintReadOnly)
	bool isAlive = true;

	UPROPERTY(BlueprintReadOnly)
	int deathPoseIndex = 0;

private:

	UFUNCTION()
	void CalculateStrafeDirection();
};
