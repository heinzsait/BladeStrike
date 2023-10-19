// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAttributeComponent::ReceiveDamage(float dmg)
{
	health = FMath::Clamp(health - dmg, 0.0f, maxHealth);	
}

float UAttributeComponent::GetHealthPercent()
{
	return (health / maxHealth);
}

bool UAttributeComponent::isAlive()
{
	if (health == 0)
		return false;
	else
		return true;
}

float UAttributeComponent::GetStaminaPercent()
{
	return (stamina / maxStamina);
}

bool UAttributeComponent::HasDodgeStamina()
{
	return (stamina > dodgeCost);
}

void UAttributeComponent::UseStaminaDodge()
{
	stamina = FMath::Clamp(stamina - dodgeCost, 0, maxStamina);
}

void UAttributeComponent::UseStaminaSprint()
{
	stamina = FMath::Clamp(stamina - sprintCost * GetWorld()->DeltaTimeSeconds, 0, maxStamina);
}

bool UAttributeComponent::HasSprintStamina()
{
	return (stamina > (sprintCost * GetWorld()->DeltaTimeSeconds));
}

void UAttributeComponent::RegenStamina(float deltaTime)
{
	stamina = FMath::Clamp(stamina + staminaRegenRate * deltaTime, 0, maxStamina);
}
