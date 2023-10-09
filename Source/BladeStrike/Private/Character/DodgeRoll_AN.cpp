// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DodgeRoll_AN.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UDodgeRoll_AN::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	character = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (character)
	{
		dodgeDir = character->GetCombatComponent()->GetDodgeDirection();
	}
}

void UDodgeRoll_AN::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (character)
	{
		dodgeDir = character->GetCombatComponent()->GetDodgeDirection();
		character->SetActorRotation(
			UKismetMathLibrary::RInterpTo_Constant(character->GetActorRotation(), dodgeDir, FrameDeltaTime, 360.0f)
			);
	}
}

void UDodgeRoll_AN::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	character->SetCharacterActionState(ECharacterActions::None);
}