// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTaskNodes/RotateToBTTaskNode.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

URotateToBTTaskNode::URotateToBTTaskNode()
{
	NodeName = TEXT("Rotate to target");
	bNotifyTick = true;
}

EBTNodeResult::Type URotateToBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();

	if (AIController)
		AIActor = Cast<AActor>(AIController->GetPawn());

	targetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(target.SelectedKeyName));

	if (AIActor && targetActor)
	{
		targetRot = UKismetMathLibrary::FindLookAtRotation(AIActor->GetActorLocation(), targetActor->GetActorLocation());
		targetRot = FRotator(AIActor->GetActorRotation().Pitch, targetRot.Yaw, AIActor->GetActorRotation().Roll);

		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::InProgress;
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Rotate task failed")));
	}
}

void URotateToBTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp,  NodeMemory, DeltaSeconds);

	if (AIActor && targetActor)
	{
		auto lerpRot = FMath::RInterpTo(AIActor->GetActorRotation(), targetRot, GetWorld()->DeltaTimeSeconds, 15);
		AIActor->SetActorRotation(lerpRot);

		if (targetRot.Equals(AIActor->GetActorRotation(), 1.0f))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

FString URotateToBTTaskNode::GetStaticDescription() const
{
	return FString("Rotate to target: " + target.SelectedKeyName.ToString());
}

