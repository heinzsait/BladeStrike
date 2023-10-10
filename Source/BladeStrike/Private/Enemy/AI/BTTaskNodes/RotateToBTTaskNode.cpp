// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTaskNodes/RotateToBTTaskNode.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Enemy.h"
#include "Kismet/KismetMathLibrary.h"

URotateToBTTaskNode::URotateToBTTaskNode()
{
	NodeName = TEXT("Rotate to target");
}

EBTNodeResult::Type URotateToBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();

	if (AIController)
		AIEnemy = Cast<AEnemy>(AIController->GetCharacter());

	targetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(target.SelectedKeyName));

	bool success = false;

	if (AIEnemy && targetActor)
	{
		auto targetRot = UKismetMathLibrary::FindLookAtRotation(AIEnemy->GetActorLocation(), targetActor->GetActorLocation());
		AIEnemy->SetActorRotation(targetRot);
		success = true;
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
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Rotate task failed")));
	}
}

FString URotateToBTTaskNode::GetStaticDescription() const
{
	return FString("Rotate to target: " + target.SelectedKeyName.ToString());
}