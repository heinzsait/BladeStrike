// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WeaponCollision_ANS.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"
#include "Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"

void UWeaponCollision_ANS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		character->GetCombatComponent()->GetMainWeapon()->SetCollision(true);
		if (character->GetCombatComponent()->GetOffHandWeapon())
		{
			character->GetCombatComponent()->GetOffHandWeapon()->SetCollision(true);
		}
	}

	AEnemy* enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (enemy)
	{
		enemy->mainWeapon->SetCollision(true);

		if (enemy->offHandWeapon)
		{
			enemy->offHandWeapon->SetCollision(true);
		}
	}
}

void UWeaponCollision_ANS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	/*AEnemy* enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (enemy)
	{
		enemy->RotateTowardsPlayer();
	}*/
}

void UWeaponCollision_ANS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AMainCharacter* character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		character->GetCombatComponent()->GetMainWeapon()->SetCollision(false);
		character->GetCombatComponent()->GetMainWeapon()->ignoreActors.Empty();
		if (character->GetCombatComponent()->GetOffHandWeapon())
		{
			character->GetCombatComponent()->GetOffHandWeapon()->SetCollision(false);
			character->GetCombatComponent()->GetOffHandWeapon()->ignoreActors.Empty();
		}
	}

	AEnemy* enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (enemy)
	{
		enemy->mainWeapon->SetCollision(false);
		enemy->mainWeapon->ignoreActors.Empty();

		if (enemy->offHandWeapon)
		{
			enemy->offHandWeapon->SetCollision(false);
			enemy->offHandWeapon->ignoreActors.Empty();
		}
	}
}
