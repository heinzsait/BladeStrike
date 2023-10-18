// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UMainOverlay;

UCLASS()
class BLADESTRIKE_API AMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	FORCEINLINE UMainOverlay* GetOverlay() { return mainOverlay; }
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainOverlay> mainOverlayClass;

	UPROPERTY()
	UMainOverlay* mainOverlay;
};
