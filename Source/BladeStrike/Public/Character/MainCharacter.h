// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "CharacterTypesh.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCurveFloat;
class AItem;

UCLASS()
class BLADESTRIKE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	bool isSprinting;

	UPROPERTY(BlueprintReadWrite)
	float inputX;

	UPROPERTY(BlueprintReadWrite)
	float inputZ;

	UPROPERTY(BlueprintReadWrite)
	float direction;

	FORCEINLINE void SetOverlappingItem(AItem* item) { overlappingItem = item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return charState; }
	FORCEINLINE EActionState GetActionState() const { return actionState; }

protected:
	virtual void BeginPlay() override;
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void Sprint(float value);
	void JumpPressed();
	void InteractPressed();
	void Attack();

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloatIncrease;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloatReset;


	FTimeline CurveTimelineReset;
	FTimeline CurveTimelineIncrease;

	UPROPERTY()
	FVector startRot;

	UPROPERTY()
	FVector endRot;

	UPROPERTY()
	float rotspeed;

	bool resetTimeline;

private:	

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	void SetDirection();

	UPROPERTY(VisibleInstanceOnly)
	AItem* overlappingItem;

	ECharacterState charState = ECharacterState::Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState actionState = EActionState::Free;

	UAnimInstance* animInstance;

	UPROPERTY(EditAnywhere)
	UAnimMontage* attackMontage;
};
