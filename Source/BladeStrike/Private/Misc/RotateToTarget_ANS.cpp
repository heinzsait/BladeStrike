// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/RotateToTarget_ANS.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"

void URotateToTarget_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	
}

void URotateToTarget_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	AEnemy* enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (enemy)
	{
		enemy->RotateTowardsPlayer();
	}
}

void URotateToTarget_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
}
