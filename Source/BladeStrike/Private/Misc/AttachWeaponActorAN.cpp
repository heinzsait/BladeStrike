// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/AttachWeaponActorAN.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"

void UAttachWeaponActorAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (attachToHand)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Purple, FString("Attaching to hand"));
		AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
		if (character)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Purple, FString("got char"));
			character->EquipMainWeapon();
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Purple, FString("Attaching to body"));
		AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
		if (character)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Purple, FString("got char"));
			character->UnEquipMainWeapon();
		}
	}
}
