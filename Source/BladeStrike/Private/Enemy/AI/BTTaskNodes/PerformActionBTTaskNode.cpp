// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTaskNodes/PerformActionBTTaskNode.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Enemy.h"

UPerformActionBTTaskNode::UPerformActionBTTaskNode()
{
	NodeName = TEXT("Perform Action");
}

EBTNodeResult::Type UPerformActionBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();

	if (AIController)
		AIEnemy = Cast<AEnemy>(AIController->GetCharacter());

	bool success = false;

	if (AIEnemy)
	{
		AIEnemy->actionState = actionToPerform;
		auto _attackTypeEnum = AIController->GetBlackboardComponent()->GetValueAsEnum(attackType.SelectedKeyName);
		float duration = AIEnemy->PerformAction((EAIAttackType)_attackTypeEnum);
		if (duration != -1)
			AIController->GetBlackboardComponent()->SetValueAsFloat(actionDuration.SelectedKeyName, duration);
		success = true;

		if (duration == -1) success = false;
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

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Perform Action task failed")));
	}
}

FString UPerformActionBTTaskNode::GetStaticDescription() const
{
	return FString::Printf(TEXT("Perform Action: %d"), actionToPerform);
}