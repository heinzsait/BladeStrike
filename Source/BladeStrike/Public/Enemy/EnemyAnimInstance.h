// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY()
	class AEnemy* enemyCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* movementComp;

public:

	UPROPERTY(BlueprintReadOnly)
	float speed;

	UPROPERTY(BlueprintReadOnly)
	float direction;
	
};
