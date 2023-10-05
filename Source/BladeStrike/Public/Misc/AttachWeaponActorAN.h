// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttachWeaponActorAN.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UAttachWeaponActorAN : public UAnimNotify
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool attachToHand;
	
};
