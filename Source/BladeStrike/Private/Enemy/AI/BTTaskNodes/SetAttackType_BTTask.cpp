// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTaskNodes/SetAttackType_BTTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Enemy.h"

USetAttackType_BTTask::USetAttackType_BTTask()
{
	NodeName = TEXT("Set Attack Type");
}

EBTNodeResult::Type USetAttackType_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();

	if (AIController)
		AIEnemy = Cast<AEnemy>(AIController->GetCharacter());

	AActor* player = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(targetPlayer.SelectedKeyName));

	bool success = false;

	if (AIEnemy && player)
	{
		EAIAttackType _attackType = EAIAttackType::Default;
		float distance = FVector::Distance(player->GetActorLocation(), AIEnemy->GetActorLocation());
		if (distance <= closeRange)
		{
			_attackType = EAIAttackType::CloseRange;
			AIController->GetBlackboardComponent()->SetValueAsEnum(attackType.SelectedKeyName, (uint8)_attackType); 
			success = true;
		}
		else if (distance > closeRange && distance <= mediumRange)
		{
			_attackType = EAIAttackType::MediumRange;
			AIController->GetBlackboardComponent()->SetValueAsEnum(attackType.SelectedKeyName, (uint8)_attackType);
			success = true;
		}
		else if (distance > mediumRange && distance <= longRange)
		{
			_attackType = EAIAttackType::MediumRange;
			AIController->GetBlackboardComponent()->SetValueAsEnum(attackType.SelectedKeyName, (uint8)_attackType);
			success = false;
		}
		else if (distance > longRange)
		{	
			_attackType = EAIAttackType::LongRange;
			AIController->GetBlackboardComponent()->SetValueAsEnum(attackType.SelectedKeyName, (uint8)_attackType);
			success = true;
		}

		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Purple, FString::Printf(TEXT("Distance = %f"), distance));*/
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

FString USetAttackType_BTTask::GetStaticDescription() const
{
	return FString::Printf(TEXT("Set Attack Type"));
}
