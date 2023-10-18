// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UMainOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* healthBar;

	UFUNCTION()
	void SetHealthPercentage(float hp);
};
