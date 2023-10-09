// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "CharacterTypesh.h"
#include "StateManagerComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnStateBegin, const FGameplayTag&, state);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnStateEnd, const FGameplayTag&, state);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCharStateBegin, const ECharacterState, state);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCharStateEnd, const ECharacterState, state);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADESTRIKE_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateManagerComponent();

	UPROPERTY()
	FOnStateBegin OnStateEnterDelegate;

	UPROPERTY()
	FOnStateEnd OnStateEndDelegate;

	UPROPERTY()
	FOnCharStateBegin OnCharStateEnterDelegate;

	UPROPERTY()
	FOnCharStateBegin OnCharStateEndDelegate;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	ECharacterState GetCurrentCharacterState();

	UFUNCTION()
	void SetCurrentCharacterState(ECharacterState state);


	UFUNCTION()
	ECharacterRotation GetCharacterRotationState();

	UFUNCTION()
	void SetCharacterRotationState(ECharacterRotation state);

	UFUNCTION()
	void ResetState();

	UFUNCTION()
	ECharacterActions GetCharacterActionState();

	UFUNCTION()
	void SetCharacterActionState(ECharacterActions state);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	UPROPERTY()
	FGameplayTag currentState;

	UFUNCTION()
	void OnStateBegin(const FGameplayTag& state);

	UFUNCTION()
	void OnStateEnded(const FGameplayTag& state);

	UPROPERTY()
	ECharacterState currentCharacterState;

	UFUNCTION()
	void OnCharStateBegin(const ECharacterState state);

	UFUNCTION()
	void OnCharStateEnded(const ECharacterState state);

	UPROPERTY()
	ECharacterRotation rotationState;

	UPROPERTY()
	ECharacterActions characterActionState;

	UPROPERTY()
	class AMainCharacter* character;
		
};
