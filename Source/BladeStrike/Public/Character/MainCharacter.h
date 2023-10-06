// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Character/CombatComponent.h"
#include "GameFramework/Character.h"
#include "CharacterTypesh.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCurveFloat;
class AItem;
class AWeapon;

UCLASS()
class BLADESTRIKE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	bool isSprinting;

	UPROPERTY(BlueprintReadWrite)
	float inputX;

	UPROPERTY(BlueprintReadWrite)
	float inputZ;

	UPROPERTY(BlueprintReadWrite)
	float direction;

	UFUNCTION()
	void EquipMainWeapon();

	UFUNCTION()
	void UnEquipMainWeapon();

	FORCEINLINE void SetOverlappingItem(AItem* item) { overlappingItem = item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return charState; }
	FORCEINLINE EActionState GetActionState() const { return actionState; }

	FORCEINLINE UCombatComponent* GetCombatComponent() const { return combatComp; }

	FORCEINLINE ECombatTypes GetCombatState() const { return combatComp->GetCombatState(); }
	FORCEINLINE void SetCombatState(ECombatTypes state) { combatComp->SetCombatState(state); }

	//FORCEINLINE AWeapon* GetMainWeapon() const { return mainWeapon; }

protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void Turn(float value);

	UFUNCTION()
	void LookUp(float value);

	UFUNCTION()
	void Sprint(float value);

	UFUNCTION()
	void JumpPressed();

	UFUNCTION()
	void InteractPressed();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void AttackToggle();


	UPROPERTY()
	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloatIncrease;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloatReset;

	UPROPERTY()
	FTimeline CurveTimelineReset;

	UPROPERTY()
	FTimeline CurveTimelineIncrease;

	UPROPERTY()
	FVector startRot;

	UPROPERTY()
	FVector endRot;

	UPROPERTY()
	float rotspeed;

	UPROPERTY()
	bool resetTimeline;

private:	

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UFUNCTION()
	void SetDirection();

	UPROPERTY(VisibleInstanceOnly)
	AItem* overlappingItem;

	UPROPERTY(EditAnywhere)
	ECharacterState charState = ECharacterState::Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState actionState = EActionState::Free;

	UPROPERTY()
	UAnimInstance* animInstance;

	UPROPERTY()
	UCombatComponent* combatComp;
};
