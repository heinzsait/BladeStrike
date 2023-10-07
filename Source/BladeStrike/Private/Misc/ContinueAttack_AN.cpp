// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/ContinueAttack_AN.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UContinueAttack_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		character->GetCombatComponent()->SetCanAttack(true);
		//character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	}
}