// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/ResetAttackCombo_AN.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"

void UResetAttackCombo_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		character->GetCombatComponent()->ResetWeapon();
	}
}
