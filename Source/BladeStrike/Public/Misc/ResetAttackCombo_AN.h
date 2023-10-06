// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ResetAttackCombo_AN.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UResetAttackCombo_AN : public UAnimNotify
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
