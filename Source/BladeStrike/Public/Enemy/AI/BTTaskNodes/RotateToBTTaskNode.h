// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RotateToBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API URotateToBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:

	URotateToBTTaskNode();

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector target;


private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY()
	class AActor* AIActor;

	UPROPERTY()
	class AAIController* AIController;

	UPROPERTY()
	AActor* targetActor;

	UPROPERTY()
	FRotator targetRot;
	
};
