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
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* healthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* staminaBar;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* YouDiedCanvas;

	UPROPERTY(meta = (BindWidget))
	class UButton* RetryButton;

	UFUNCTION()
	void SetHealthPercentage(float hp);

	UFUNCTION()
	void SetStaminaPercentage(float stamina);

	UFUNCTION()
	void ShowYouDied();

	UFUNCTION()
	void HideYouDied();

private:

	UFUNCTION()
	void OnRetryClicked();
};
