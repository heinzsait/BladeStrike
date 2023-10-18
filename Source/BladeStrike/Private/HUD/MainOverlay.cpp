// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	RetryButton->OnClicked.AddUniqueDynamic(this, &UMainOverlay::OnRetryClicked);
}

void UMainOverlay::SetHealthPercentage(float hp)
{
	healthBar->SetPercent(hp);
}

void UMainOverlay::ShowYouDied()
{
	YouDiedCanvas->SetVisibility(ESlateVisibility::Visible);
}

void UMainOverlay::HideYouDied()
{
	YouDiedCanvas->SetVisibility(ESlateVisibility::Hidden);
}

void UMainOverlay::OnRetryClicked()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}