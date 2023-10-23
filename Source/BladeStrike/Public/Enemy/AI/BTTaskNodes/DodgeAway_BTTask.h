// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DodgeAway_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API UDodgeAway_BTTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UDodgeAway_BTTask();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	
};
