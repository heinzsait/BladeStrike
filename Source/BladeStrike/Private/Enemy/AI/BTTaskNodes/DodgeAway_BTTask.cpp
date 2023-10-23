// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTaskNodes/DodgeAway_BTTask.h"
#include "GameManager.h"

UDodgeAway_BTTask::UDodgeAway_BTTask()
{
	NodeName = TEXT("Should Perform Dodge");
}

EBTNodeResult::Type UDodgeAway_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	bool success = false;
	if (GetWorld())
	{
		success = GetWorld()->GetSubsystem<UGameManager>()->IsPlayerAttacking();
	}

	if (success)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
}

FString UDodgeAway_BTTask::GetStaticDescription() const
{
	return FString("Should Perform Dodge");
}
