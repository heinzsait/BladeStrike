// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "EnumClass.h"
#include "Character/CombatComponent.h"
#include "Character/StateManagerComponent.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCurveFloat;
class AItem;
class AWeapon;
class UTargetingComponent;

UCLASS()
class BLADESTRIKE_API AMainCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AMainCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& impactPoint) override;

	UPROPERTY(EditAnywhere)
	UAnimMontage* hitReactionMontages;

	UPROPERTY(BlueprintReadWrite)
	bool isSprinting;

	UPROPERTY(BlueprintReadWrite)
	float inputX;

	UPROPERTY(BlueprintReadWrite)
	float inputZ;

	UPROPERTY(BlueprintReadWrite)
	float direction;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UFUNCTION()
	void EquipMainWeapon();

	UFUNCTION()
	void UnEquipMainWeapon();

	UFUNCTION()
	void Block();

	UFUNCTION()
	void UnBlock();

	FORCEINLINE void SetOverlappingItem(AItem* item) { overlappingItem = item; }
	//FORCEINLINE ECharacterState GetCharacterState() const { return charState; }

	FORCEINLINE UCombatComponent* GetCombatComponent() const { return combatComp; }
	FORCEINLINE ECombatTypes GetCombatState() const { return combatComp->GetCombatState(); }
	FORCEINLINE void SetCombatState(ECombatTypes state) { combatComp->SetCombatState(state); }

	FORCEINLINE UStateManagerComponent* GetStateManagerComponent() const { return stateManager; }
	FORCEINLINE ECharacterState GetCharacterState() const { return stateManager->GetCurrentCharacterState(); }
	FORCEINLINE void SetCharacterState(ECharacterState state) { stateManager->SetCurrentCharacterState(state); }

	FORCEINLINE ECharacterRotation GetRotationState() const { return stateManager->GetCharacterRotationState(); }

	UFUNCTION()
	bool IsTargetLocked() const;

	UFUNCTION()
	void SetCharacterActionState(ECharacterActions state);

	UFUNCTION()
	ECharacterActions GetCharacterActionState();

	UFUNCTION()
	USpringArmComponent* GetCameraBoom();

	UFUNCTION()
	void ResetTargetLock(AActor* _enemy = nullptr);

	//FORCEINLINE AWeapon* GetMainWeapon() const { return mainWeapon; }

protected:
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

	UFUNCTION()
	void Dodge();

	UFUNCTION()
	void LockTarget();


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

	UFUNCTION()
	void SetDirection();

	UPROPERTY(VisibleInstanceOnly)
	AItem* overlappingItem;

	//UPROPERTY(EditAnywhere)
	//ECharacterState charState = ECharacterState::Unequipped;

	UPROPERTY()
	UAnimInstance* animInstance;

	UPROPERTY(EditAnywhere)
	UCombatComponent* combatComp;

	UPROPERTY()
	UStateManagerComponent* stateManager;

	UPROPERTY()
	UTargetingComponent* targetingComp;


	UFUNCTION()
	void DirectionalHitReact(const FVector& impactPoint);

	UFUNCTION()
	void PlayHitReaction(const FName sectionName);
};
