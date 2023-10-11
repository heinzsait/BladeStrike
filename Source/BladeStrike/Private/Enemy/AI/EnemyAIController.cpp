// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/EnemyAIController.h"
#include "Enemy/Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Character/MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	perceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	enemy = Cast<AEnemy>(InPawn);
	if (enemy)
	{
		RunBehaviorTree(enemy->behaviourTree);
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	perceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPlayerDetected);
}

void AEnemyAIController::OnPlayerDetected(const TArray<AActor*>& DetectedPawn)
{
	for (AActor* actor : DetectedPawn)
	{
		FActorPerceptionBlueprintInfo info;
		perceptionComp->GetActorsPerception(actor, info);

		if (actor->ActorHasTag("Player"))
		{
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, FString("Sensed actor : " + actor->GetFName().ToString()));*/
			for (FAIStimulus aiStimuli : info.LastSensedStimuli)
			{
				if (aiStimuli.WasSuccessfullySensed())
				{
					GetBlackboardComponent()->SetValueAsObject(FName("Target"), actor);
				}
			}
		}
	}
}

void AEnemyAIController::Die()
{
	BrainComponent->StopLogic(FString("Dead"));
}
