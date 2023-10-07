// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DodgeEnd_AN.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UDodgeEnd_AN : public UAnimNotify
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
