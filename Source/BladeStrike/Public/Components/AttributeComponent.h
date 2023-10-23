// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADESTRIKE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	UFUNCTION()
	void ReceiveDamage(float dmg);

	UFUNCTION()
	void RegenFullHealth();

	UFUNCTION()
	float GetHealthPercent();

	UFUNCTION()
	bool isAlive();

	UFUNCTION()
	float GetStaminaPercent();

	UFUNCTION()
	bool HasDodgeStamina();

	UFUNCTION()
	void UseStaminaDodge();

	UFUNCTION()
	void UseStaminaSprint();

	UFUNCTION()
	bool HasSprintStamina();

	UFUNCTION()
	void RegenStamina(float deltaTime);

	UFUNCTION()
	void Heal(float hp);

	UFUNCTION()
	bool CanHeal();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float health = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float maxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float stamina = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float maxStamina = 100;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float staminaRegenRate = 5;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float dodgeCost = 20;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float sprintCost = 7;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int healPotionCount = 2;
};
