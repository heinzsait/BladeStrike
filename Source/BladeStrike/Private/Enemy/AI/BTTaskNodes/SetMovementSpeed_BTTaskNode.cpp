// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTaskNodes/SetMovementSpeed_BTTaskNode.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USetMovementSpeed_BTTaskNode::USetMovementSpeed_BTTaskNode()
{
	NodeName = TEXT("Set Movement Speed");
}

EBTNodeResult::Type USetMovementSpeed_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if(AIController)
		AICharacter = AIController->GetCharacter();

	if (AICharacter)
	{
		AICharacter->GetCharacterMovement()->MaxWalkSpeed = movementSpeed;

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("Set Movement Speed task failed")));
	}
}

FString USetMovementSpeed_BTTaskNode::GetStaticDescription() const
{
	return FString::Printf(TEXT("Movement Speed : %f"), movementSpeed);
}
