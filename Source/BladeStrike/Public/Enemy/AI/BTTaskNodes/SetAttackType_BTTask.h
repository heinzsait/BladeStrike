// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetAttackType_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API USetAttackType_BTTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USetAttackType_BTTask();

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector attackType;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector targetPlayer;

	UPROPERTY(EditAnywhere)
	float closeRange = 150;

	UPROPERTY(EditAnywhere)
	float mediumRange = 300;

	UPROPERTY(EditAnywhere)
	float longRange = 400;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY()
	class AEnemy* AIEnemy;

	UPROPERTY()
	class AAIController* AIController;
};
