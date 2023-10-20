// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/AttachWeaponActorAN.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"

void UAttachWeaponActorAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (attachToHand)
	{
		AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
		if (character)
		{
			character->EquipMainWeapon();
		}
	}
	else
	{
		AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
		if (character)
		{
			character->UnEquipMainWeapon();
		}
	}
}
