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
		bool sucess = RunBehaviorTree(enemy->behaviourTree);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString("running BT: " + enemy->behaviourTree->GetFName().ToString()));
		
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	perceptionComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPlayerDetected);
}

void AEnemyAIController::OnPlayerDetected(const TArray<AActor*>& DetectedPawn)
{
	//GetBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);
	for (AActor* actor : DetectedPawn)
	{
		FActorPerceptionBlueprintInfo info;
		perceptionComp->GetActorsPerception(actor, info);

		auto aiStimuli = info.LastSensedStimuli[0];
		if (aiStimuli.WasSuccessfullySensed())
		{
			AMainCharacter* player = Cast<AMainCharacter>(actor);
			if (player)
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString("Sensed player"));

				GetBlackboardComponent()->SetValueAsObject(FName("Target"), player);
			}
		}
	}
}

