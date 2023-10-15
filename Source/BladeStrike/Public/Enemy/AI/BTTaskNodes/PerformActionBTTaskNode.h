// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnumClass.h"
#include "PerformActionBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UPerformActionBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPerformActionBTTaskNode();

	UPROPERTY(EditAnywhere)
	ECharacterActions actionToPerform;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector actionDuration;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector attackType;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY()
	class AEnemy* AIEnemy;

	UPROPERTY()
	class AAIController* AIController;
	
};
