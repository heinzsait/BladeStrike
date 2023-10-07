// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/StateManagerComponent.h"
#include "GameplayTagContainer.h"
#include "Character/CharacterTypesh.h"

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

void UStateManagerComponent::ResetState()
{

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

