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
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void RotateTowardsPlayer();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& impactPoint) override;

	UFUNCTION()
	void DirectionalHitReact(const FVector& impactPoint);

	UFUNCTION()
	void PlayHitReaction(const FName sectionName);

	UFUNCTION()
	float PerformAction();

	UFUNCTION()
	float PerformAttack();

	UPROPERTY(EditAnywhere)
	UAnimMontage* hitReactionMontages;

	//UPROPERTY(EditAnywhere)
	//TArray<UAnimMontage*> attackMontages;

	UPROPERTY(BlueprintReadOnly)
	class AMainCharacter* player;

	UPROPERTY(EditAnywhere)
	TArray<class ATargetPoint*> patrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* behaviourTree;

	UPROPERTY(EditAnywhere)
	ECharacterState currentState = ECharacterState::Unequipped;

	UPROPERTY(EditAnywhere)
	ECharacterActions actionState = ECharacterActions::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWeapon* mainWeapon;

	/*UPROPERTY()
	FTakePointDamageSignature TakePointDamage;

	virtual void TakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser ) override;*/

};
