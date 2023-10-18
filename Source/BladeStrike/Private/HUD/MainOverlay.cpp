// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainOverlay.h"
#include "Components/ProgressBar.h"

void UMainOverlay::SetHealthPercentage(float hp)
{
	healthBar->SetPercent(hp);
}
