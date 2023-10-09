// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/StateManagerComponent.h"
#include "GameplayTagContainer.h"
#include "Character/CharacterTypesh.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UStateManagerComponent::UStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//OnStateEnterDelegate.BindUFunction(this, FName("OnStateBegin"));
	//OnStateEndDelegate.BindUFunction(this, FName("OnStateEnded"));

	OnStateEnterDelegate.BindDynamic(this, &UStateManagerComponent::OnStateBegin);
	OnStateEndDelegate.BindDynamic(this, &UStateManagerComponent::OnStateEnded);

	OnCharStateEnterDelegate.BindDynamic(this, &UStateManagerComponent::OnCharStateBegin);
	OnCharStateEndDelegate.BindDynamic(this, &UStateManagerComponent::OnCharStateEnded);

	rotationState = ECharacterRotation::Movement;
	character = Cast<AMainCharacter>(GetOwner());
	
}


// Called every frame
void UStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ECharacterState UStateManagerComponent::GetCurrentCharacterState()
{
	return currentCharacterState;
}

void UStateManagerComponent::SetCurrentCharacterState(ECharacterState state)
{
	if (currentCharacterState != state)
	{
		OnCharStateEndDelegate.Execute(state);
		currentCharacterState = state;
		OnCharStateEnterDelegate.Execute(currentCharacterState);
	}
}

ECharacterRotation UStateManagerComponent::GetCharacterRotationState()
{
	return rotationState;
}

void UStateManagerComponent::SetCharacterRotationState(ECharacterRotation state)
{
	rotationState = state;
	switch (rotationState)
	{
	case ECharacterRotation::Camera:
		//character->bUseControllerRotationYaw = false;
		character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		character->GetCharacterMovement()->bOrientRotationToMovement = false;
		//character->GetCameraBoom()->bUsePawnControlRotation = false;
		break;

	case ECharacterRotation::Movement:
		//character->bUseControllerRotationYaw = false;
		character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		character->GetCharacterMovement()->bOrientRotationToMovement = true;
		//character->GetCameraBoom()->bUsePawnControlRotation = true;

		break;
	default:
		break;
	}
}

void UStateManagerComponent::ResetState()
{

}

ECharacterActions UStateManagerComponent::GetCharacterActionState()
{
	return characterActionState;
}

void UStateManagerComponent::SetCharacterActionState(ECharacterActions state)
{
	characterActionState = state;
}

void UStateManagerComponent::OnStateBegin(const FGameplayTag& state)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString("Entering state: " + state.ToString()));
	}
}

void UStateManagerComponent::OnStateEnded(const FGameplayTag& state)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString("Exiting state: " + state.ToString()));
	}
}

void UStateManagerComponent::OnCharStateBegin(const ECharacterState state)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Red, FString("Entering state: " + int(state)));
}

void UStateManagerComponent::OnCharStateEnded(const ECharacterState state)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Red, FString("Exiting state: " + int(state)));
}

