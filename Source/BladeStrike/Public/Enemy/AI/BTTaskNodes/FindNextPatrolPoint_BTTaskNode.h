// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FindNextPatrolPoint_BTTaskNode.generated.h"

UCLASS()
class BLADESTRIKE_API UFindNextPatrolPoint_BTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UFindNextPatrolPoint_BTTaskNode();

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector targetLocation;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector patrolIndex;

	


private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UFUNCTION()
	void IncrementPatrolIndex(AAIController* AIController);
	UFUNCTION()
	void DecrementPatrolIndex(AAIController* AIController);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn", meta = (AllowPrivateAccess = true))
	class AEnemy* AIEnemy;

	UFUNCTION()
	void SetPathLocation(AAIController* AIController, bool& success);

	UPROPERTY()
	bool reverse = false;
	
};
