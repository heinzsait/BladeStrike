// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTaskNodes/FindNextPatrolPoint_BTTaskNode.h"
#include "AIController.h"
#include "Enemy/Enemy.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"

UFindNextPatrolPoint_BTTaskNode::UFindNextPatrolPoint_BTTaskNode()
{
	NodeName = TEXT("Find Next Patrol Point");
}

EBTNodeResult::Type UFindNextPatrolPoint_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController)
		AIEnemy = Cast<AEnemy>(AIController->GetCharacter());

	bool success = false;

	SetPathLocation(AIController, success);

	if (success)
	{
		if (!reverse)
			IncrementPatrolIndex(AIController);
		else
			DecrementPatrolIndex(AIController);
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
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Find Next Patrol Point task failed")));
	}
}

void UFindNextPatrolPoint_BTTaskNode::IncrementPatrolIndex(AAIController* AIController)
{
	if (AIEnemy)
	{
		int i = AIController->GetBlackboardComponent()->GetValueAsInt(patrolIndex.SelectedKeyName);

		if ((AIEnemy->patrolPoints.Num() - 1) > i)
			i++;
		else
			reverse = true;

		AIController->GetBlackboardComponent()->SetValueAsInt(patrolIndex.SelectedKeyName, i);
	}
}

void UFindNextPatrolPoint_BTTaskNode::DecrementPatrolIndex(AAIController* AIController)
{
	if (AIEnemy)
	{
		int i = AIController->GetBlackboardComponent()->GetValueAsInt(patrolIndex.SelectedKeyName);

		if (i > 0)
			i--;
		else
			reverse = false;

		AIController->GetBlackboardComponent()->SetValueAsInt(patrolIndex.SelectedKeyName, i);
	}
}

void UFindNextPatrolPoint_BTTaskNode::SetPathLocation(AAIController* AIController, bool& success)
{
	FNavLocation Location{};
	if (AIEnemy)
	{
		int index = AIController->GetBlackboardComponent()->GetValueAsInt(patrolIndex.SelectedKeyName);
		ATargetPoint* point = AIEnemy->patrolPoints[index];

		if (point)
		{
			const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			if (IsValid(NavSystem) && NavSystem->ProjectPointToNavigation(point->GetActorLocation(), Location))
			{
				AIController->GetBlackboardComponent()->SetValueAsVector(targetLocation.SelectedKeyName, Location.Location);
				success = true;
			}
		}
	}
}

FString UFindNextPatrolPoint_BTTaskNode::GetStaticDescription() const
{
	return FString("");//::Printf(TEXT("Patrol Point : %d"), movementSpeed);
}


