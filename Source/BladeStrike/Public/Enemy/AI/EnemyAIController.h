// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerDetected(const TArray<AActor*>& DetectedPawn);

private:
	class AEnemy* enemy;

	UPROPERTY(EditAnywhere)
	class UAIPerceptionComponent* perceptionComp;
};
