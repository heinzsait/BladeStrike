// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameManager.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UGameManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable)
	void PlayWorldMusic();

	UFUNCTION(BlueprintCallable)
	void StopWorldMusic();

	UFUNCTION(BlueprintCallable)
	void PlayBossMusic();

	UFUNCTION(BlueprintCallable)
	void StopBossMusic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* levelMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* bossMusic;

	UFUNCTION()
	void SetPlayerAttacking(bool flag);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsPlayerAttacking();

	UFUNCTION(BlueprintCallable)
	void SetCheckpoint(FVector location);

	UFUNCTION(BlueprintCallable)
	FVector GetCheckpoint();

	UFUNCTION()
	class AMainCharacter* GetPlayer();

private:
	bool isBossMusicPlaying = false;
	bool isLevelMusicPlaying = false;

	UAudioComponent* levelMusicComp;
	UAudioComponent* bossMusicComp;

	bool playerAttacking = false;

	bool checkpointSet = false;
	FVector lastCheckpointLocation = FVector::ZeroVector;
};
