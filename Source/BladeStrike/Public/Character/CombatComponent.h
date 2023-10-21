// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnumClass.h"
#include "CombatComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADESTRIKE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	FORCEINLINE ECombatTypes GetCombatState() const { return combatType; }
	FORCEINLINE void SetCombatState(ECombatTypes state) { combatType = state; }

	FORCEINLINE AWeapon* GetMainWeapon() const { return mainWeapon; }
	FORCEINLINE void SetMainWeapon(AWeapon* weapon) { mainWeapon = weapon; }

	FORCEINLINE AWeapon* GetOffHandWeapon() const { return offHandWeapon; }
	FORCEINLINE void SetOffHandWeapon(AWeapon* weapon) { offHandWeapon = weapon; }

	//FORCEINLINE void SaveAttack() { attackSaved = true; }
	//FORCEINLINE void ResetSavedAttack() { attackSaved = false; }
	//FORCEINLINE bool GetSavedAttack() const { return attackSaved; }

	//FORCEINLINE void SetCanAttack(bool flag) { canAttack = flag; }
	//FORCEINLINE bool CanAttack() const { return canAttack; }

	UFUNCTION()
	void ResetWeapon();

	UFUNCTION()
	void PerformAttack();

	UFUNCTION()
	void SelectNextAttack();

	UFUNCTION()
	void PerformDodge();

	UFUNCTION()
	FRotator GetDodgeDirection();

	UPROPERTY(EditAnywhere)
	UAnimMontage* unarmedDodgeMontage;

	UPROPERTY(EditAnywhere)
	USoundBase* attackExertionSFX;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	ECombatTypes combatType = ECombatTypes::None;

	UPROPERTY()
	AWeapon* mainWeapon;

	UPROPERTY()
	AWeapon* offHandWeapon;

	//UPROPERTY()
	//bool attackSaved;

	//UPROPERTY()
	//bool canAttack = true;

	UPROPERTY()
	UAnimInstance* animInstance;

	UPROPERTY()
	class AMainCharacter* character;
};
