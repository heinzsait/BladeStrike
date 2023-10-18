// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "EnumClass.h"
#include "Enemy.generated.h"

UCLASS()
class BLADESTRIKE_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void RotateTowardsPlayer();

	virtual void GetHit(const FVector& impactPoint) override;

	UFUNCTION()
	void ShowHealthBar();

	UFUNCTION()
	void HideHealthBar();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
	UFUNCTION()
	float PerformAction(EAIAttackType _attackType = EAIAttackType::Default);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* behaviourTree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWeapon* mainWeapon; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWeapon* offHandWeapon;
	
	UPROPERTY(EditAnywhere)
	ECharacterState currentState = ECharacterState::Unequipped;

	UPROPERTY(EditAnywhere)
	ECharacterActions actionState = ECharacterActions::None;

	UPROPERTY(EditAnywhere)
	EAIAttackType attackType = EAIAttackType::Default;

	UPROPERTY(EditAnywhere)
	TArray<class ATargetPoint*> patrolPoints;

	UFUNCTION()
	bool isAlive();

	UFUNCTION()
	FVector GetTranslationWrapTarget();

	UFUNCTION()
	FRotator GetRotationWrapTarget();

	UFUNCTION()
	void UpdateMotionWrapping();

	UPROPERTY(EditAnywhere)
	float wrapTargetDistance = 75.0f;

protected:
	virtual void BeginPlay() override;


private:	
	

	UFUNCTION()
	float PerformAttack(EAIAttackType _attackType = EAIAttackType::Default);

	UPROPERTY()
	EAIAttackType currentAttackType;

	UFUNCTION()
	void DirectionalHitReact(const FVector& impactPoint);

	UFUNCTION()
	void PlayHitReaction(const FName sectionName);

	UFUNCTION()
	void Die();


	UPROPERTY(EditAnywhere)
	UAnimMontage* hitReactionMontages;

	UPROPERTY(EditAnywhere)
	class UAttributeComponent* attributes;

	UPROPERTY()
	class AMainCharacter* player;

	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* healthBarWidget;

	UPROPERTY()
	class UMotionWarpingComponent* motionWrapComp;
	
	UPROPERTY()
	FVector hitImpactPoint;	

	//For Boss
	UPROPERTY(EditAnywhere, category = "Boss")
	TArray<UAnimMontage*> closeRangeAttacks;

	UPROPERTY(EditAnywhere, category = "Boss")
	TArray<UAnimMontage*> mediumRangeAttacks;

	UPROPERTY(EditAnywhere, category = "Boss")
	TArray<UAnimMontage*> longRangeAttacks;
};
