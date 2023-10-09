// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TargetingComponent.h"
#include "Character/MainCharacter.h"
#include "Enemy/Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	character = Cast<AMainCharacter>(GetOwner());
	if (character)
	{
		camera = character->GetComponentByClass<UCameraComponent>();
	}
}


AActor* UTargetingComponent::FindTarget()
{

	if (!character || !camera) return nullptr;

	AActor* target = nullptr;
	FVector start = character->GetActorLocation();
	float distance = 700.0f;
	float radius = 200.0f;
	FVector end = (distance * camera->GetForwardVector()) + start;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(character);
	FHitResult hitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray; // object types to trace
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	UKismetSystemLibrary::SphereTraceSingleForObjects(this, start, end, radius, objectTypesArray, false, actorsToIgnore, EDrawDebugTrace::ForDuration, hitResult, true);

	if (hitResult.GetActor())
	{
		AEnemy* enemy = Cast<AEnemy>(hitResult.GetActor());
		if (enemy)
		{
			target = enemy;
		}
	}

	return target;
}

void UTargetingComponent::LockTarget()
{
	if (isLocked)
	{
		//Unlock
		isLocked = false;
		lockedOnTarget = nullptr;
		character->GetStateManagerComponent()->SetCharacterRotationState(ECharacterRotation::Movement);
	}
	else
	{
		AActor* target = FindTarget();
		if (target)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString("target = " + target->GetName()));

			lockedOnTarget = target;
			isLocked = true;

			character->GetStateManagerComponent()->SetCharacterRotationState(ECharacterRotation::Camera);
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString("no target found"));

			lockedOnTarget = nullptr;
		}
	}
}

void UTargetingComponent::UpdateTargetRotation()
{
	if (!lockedOnTarget || !character) return;
	if (character->GetCharacterActionState() == ECharacterActions::Dodging) return;

	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(character->GetActorLocation() + FVector(0.0f, 0.0f, 200.0f), lockedOnTarget->GetActorLocation());
	//FRotator lerpRot = UKismetMathLibrary::RLerp(character->GetActorRotation(), targetRot, GetWorld()->DeltaTimeSeconds, true);
	FRotator newRot = FRotator(character->Controller->GetControlRotation().Pitch, targetRot.Yaw, targetRot.Roll);
	character->Controller->SetControlRotation(newRot);

	if (FVector::Distance(lockedOnTarget->GetActorLocation(), character->GetActorLocation()) > 1500 || character->GetCharacterState() != ECharacterState::Equipped)
	{
		//Unlock
		isLocked = false;
		lockedOnTarget = nullptr;
		character->GetStateManagerComponent()->SetCharacterRotationState(ECharacterRotation::Movement);
	}
}

// Called every frame
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isLocked && lockedOnTarget)
	{
		UpdateTargetRotation();
	}
}

