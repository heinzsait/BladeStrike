// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	enemyCharacter = Cast<AEnemy>(TryGetPawnOwner());
	if (enemyCharacter)
	{
		movementComp = enemyCharacter->GetCharacterMovement();
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (enemyCharacter)
	{
		direction = CalculateDirection(enemyCharacter->GetCharacterMovement()->Velocity, enemyCharacter->GetActorRotation());
		speed = UKismetMathLibrary::VSizeXY(enemyCharacter->GetCharacterMovement()->Velocity);
	}
}

void UEnemyAnimInstance::Die()
{
	isAlive = false;
	deathPoseIndex = FMath::RandRange(0, 10);
}
