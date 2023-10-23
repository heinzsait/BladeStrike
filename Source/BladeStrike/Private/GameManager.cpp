// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TStatId UGameManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UGameManager, STATGROUP_Tickables);
}

void UGameManager::PlayWorldMusic()
{
	if (!isLevelMusicPlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayWorldMusic"));
		isLevelMusicPlaying = true;

		if (levelMusic)
		{
			levelMusicComp = UGameplayStatics::SpawnSound2D(this, levelMusic);
		}
	}
}

void UGameManager::StopWorldMusic()
{
	if (isLevelMusicPlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("StopWorldMusic"));
		isLevelMusicPlaying = false;

		if (levelMusicComp)
		{
			levelMusicComp->Stop();
		}
	}
}

void UGameManager::PlayBossMusic()
{
	if (!isBossMusicPlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayBossMusic"));
		isBossMusicPlaying = true;

		if (bossMusic)
		{
			bossMusicComp = UGameplayStatics::SpawnSound2D(this, bossMusic);
		}
	}
}

void UGameManager::StopBossMusic()
{
	if (isBossMusicPlaying)
	{
		UE_LOG(LogTemp, Warning, TEXT("StopBossMusic"));
		isBossMusicPlaying = false;

		if (bossMusicComp)
		{
			bossMusicComp->Stop();
		}
	}
}

void UGameManager::SetPlayerAttacking(bool flag)
{
	playerAttacking = flag;
}

bool UGameManager::IsPlayerAttacking()
{
	return playerAttacking;
}

void UGameManager::SetCheckpoint(FVector location)
{
	checkpointSet = true;
	lastCheckpointLocation = location;
}

FVector UGameManager::GetCheckpoint()
{
	if (checkpointSet)
		return lastCheckpointLocation;
	else
		return FVector::ZeroVector;
}

AMainCharacter* UGameManager::GetPlayer()
{
	AMainCharacter* player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	return player;
}
