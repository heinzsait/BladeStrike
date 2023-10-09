// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/EnemyAIController.h"
#include "Enemy/Enemy.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIController::AEnemyAIController()
{

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
