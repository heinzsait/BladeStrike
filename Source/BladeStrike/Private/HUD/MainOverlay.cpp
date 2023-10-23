// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"
#include "Character/MainCharacter.h"

void UMainOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	RetryButton->OnClicked.AddUniqueDynamic(this, &UMainOverlay::OnRetryClicked);
}

void UMainOverlay::SetHealthPercentage(float hp)
{
	healthBar->SetPercent(hp);
}

void UMainOverlay::SetStaminaPercentage(float stamina)
{
	staminaBar->SetPercent(stamina);
}

void UMainOverlay::SetBossHealthPercentage(float hp)
{
	bossHealthBar->SetPercent(hp);
}

void UMainOverlay::ShowBossHealth()
{
	bossHealthBar->SetVisibility(ESlateVisibility::Visible);

	UGameManager* GM = GetWorld()->GetSubsystem<UGameManager>();
	if (GM)
	{
		GM->StopWorldMusic();
		GM->PlayBossMusic();
	}
}

void UMainOverlay::HideBossHealth()
{
	bossHealthBar->SetVisibility(ESlateVisibility::Hidden);

	UGameManager* GM = GetWorld()->GetSubsystem<UGameManager>();
	if (GM)
	{
		GM->StopBossMusic();
		GM->PlayWorldMusic();
	}
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
	UGameManager* GM = GetWorld()->GetSubsystem<UGameManager>();
	if (GM)
	{
		if (GM->GetCheckpoint() != FVector::ZeroVector)
		{
			AMainCharacter* player = GM->GetPlayer();
			if (player)
			{
				player->Respawn(GM->GetCheckpoint());
			}
		}
		else
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
	else
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}