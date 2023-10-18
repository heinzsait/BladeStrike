// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainHUD.h"
#include "HUD/MainOverlay.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		APlayerController* controller = GetWorld()->GetFirstPlayerController();
		if (controller && mainOverlayClass)
		{
			mainOverlay = CreateWidget<UMainOverlay>(controller, mainOverlayClass);
			mainOverlay->AddToViewport();
		}
	}
}
