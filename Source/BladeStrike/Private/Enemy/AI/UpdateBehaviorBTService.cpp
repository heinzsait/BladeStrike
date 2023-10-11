// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/UpdateBehaviorBTService.h"
#include "Enemy/Enemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnumClass.h"

UUpdateBehaviorBTService::UUpdateBehaviorBTService(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{
	NodeName = "Update Behavior";

	bNotifyBecomeRelevant = true;	
	Interval = 0.5f;
	RandomDeviation = 0.f;
}

void UUpdateBehaviorBTService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UUpdateBehaviorBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AIController = OwnerComp.GetAIOwner();

	if (AIController)
		enemy = Cast<AEnemy>(AIController->GetCharacter());

	UpdateBehavior();
}


void UUpdateBehaviorBTService::UpdateBehavior()
{
	if (enemy && AIController)
	{
		if (enemy->currentState != ECharacterState::Dead)
		{
			AActor* targetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(targetKey.SelectedKeyName));
			if (targetActor)
			{
				//Can see the player...
				enemy->ShowHealthBar();
				canSeePlayer = true;
				float distance = FVector::Distance(enemy->GetActorLocation(), targetActor->GetActorLocation());
				if (distance <= attackingRange)
				{
					//Player within range so attack...
					SetBehavior(EAIBehaviors::Attack);
				}
				else
				{
					if (distance < loseInterestRange)
					{
						//Player not in range so chase...
						SetBehavior(EAIBehaviors::Chase);
					}
					else
					{
						//Cannot see the player...
						SetBehavior(EAIBehaviors::Patrol);
						enemy->HideHealthBar();
					}
				}
			}
			else
			{
				//Cannot see the player...
				SetBehavior(EAIBehaviors::Patrol);
			}
		}
		else
		{
			SetBehavior(EAIBehaviors::None);
		}
	}
}

void UUpdateBehaviorBTService::SetBehavior(EAIBehaviors newBehavior)
{
	AIController->GetBlackboardComponent()->SetValueAsEnum(behaviorKey.SelectedKeyName, (uint8)newBehavior);
	if (newBehavior == EAIBehaviors::Patrol)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(targetKey.SelectedKeyName, nullptr);
	}
}