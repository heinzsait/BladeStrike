// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/AttackAnimRotation_ANS.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAttackAnimRotation_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}
}

void UAttackAnimRotation_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UAttackAnimRotation_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}
}


