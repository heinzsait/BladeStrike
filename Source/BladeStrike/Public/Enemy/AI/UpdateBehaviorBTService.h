// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UpdateBehaviorBTService.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UUpdateBehaviorBTService : public UBTService
{
	GENERATED_BODY()
	
public:

	UUpdateBehaviorBTService(const FObjectInitializer& ObjectInitializer);

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void UpdateBehavior();

	UFUNCTION()
	void SetBehavior(EAIBehaviors newBehavior);

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector behaviorKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector targetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector isAnyMontagePlayingKey;

	UPROPERTY(EditAnywhere)
	float attackingRange = 500;

	UPROPERTY(EditAnywhere)
	float loseInterestRange = 1500;


private:

	UPROPERTY()
	class AEnemy* enemy;

	UPROPERTY()
	class AAIController* AIController;

	UPROPERTY()
	bool canSeePlayer;

	UPROPERTY()
	bool playerDead;
};
