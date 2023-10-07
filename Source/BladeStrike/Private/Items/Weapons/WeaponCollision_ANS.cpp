// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponCollision_ANS.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"

void UWeaponCollision_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		character->GetCombatComponent()->GetMainWeapon()->SetCollision(true);
	}
}

void UWeaponCollision_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

}

void UWeaponCollision_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		character->GetCombatComponent()->GetMainWeapon()->SetCollision(false);
	}
}
