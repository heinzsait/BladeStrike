// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetMovementSpeed_BTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API USetMovementSpeed_BTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USetMovementSpeed_BTTaskNode();

	UPROPERTY(EditAnywhere)
	float movementSpeed = 400.f;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn", meta = (AllowPrivateAccess = true))
	ACharacter* AICharacter;
	
};
