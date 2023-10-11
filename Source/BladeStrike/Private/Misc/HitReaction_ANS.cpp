// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/HitReaction_ANS.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"

void UHitReaction_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	
}

void UHitReaction_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UHitReaction_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		if (character->isBlockKeyDown)
			character->SetCharacterActionState(ECharacterActions::Blocking);
		else
			character->SetCharacterActionState(ECharacterActions::None);
	}

	AEnemy* enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (enemy)
	{
		enemy->actionState = ECharacterActions::None;
	}
}

