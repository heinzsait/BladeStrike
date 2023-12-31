// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class BLADESTRIKE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(EditAnywhere)
	bool isWeaponEnemy = false;

	UFUNCTION()
	void Equip(USceneComponent* InParent);

	void CreateOffHandShield(AMainCharacter* player, USceneComponent* InParent);

	UFUNCTION()
	void UnEquip(USceneComponent* InParent);

	UFUNCTION()
	void DropWeapon();

	UFUNCTION()
	AWeapon* Clone();

	UFUNCTION()
	AWeapon* CreateShield();

	UPROPERTY(EditAnywhere)
	float weaponDamage = 10.0f;

	UPROPERTY(EditAnywhere)
	bool isOffHanded = false;

	UPROPERTY(EditAnywhere)
	bool hasShield = false;

	UPROPERTY(EditAnywhere)
	bool isShield = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> shieldToSpawn;

	UPROPERTY(EditAnywhere)
	UAnimMontage* swordDrawMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* swordDisarmMontage;

	UPROPERTY(EditAnywhere)
	ECombatTypes combatType;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> attackMontages;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> heavyAttackMontages;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> comboAttackMontages;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> fallingAttackMontages;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> sprintAttackMontages;

	UPROPERTY(EditAnywhere)
	UAnimMontage* sprintJumpAttackMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* dodgeMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* blockHitMontage;

	UPROPERTY(EditAnywhere)
	UParticleSystem* hitParticles;

	UPROPERTY(EditAnywhere)
	UParticleSystem* hitParticlesEnemy;

	UPROPERTY(EditAnywhere)
	int attackIndex = 0;

	FORCEINLINE void SetCollision(bool flag) { collisionEnabled = flag; }
	FORCEINLINE void ResetWeapon() { attackIndex = 0; }

	UPROPERTY()
	TArray<AActor*> ignoreActors;

	UPROPERTY()
	AWeapon* offHandPair;

	UPROPERTY()
	AWeapon* mainHandPair;

	UPROPERTY()
	bool isAttached = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:


	UPROPERTY(EditAnywhere)
	FName attachSocket;

	UPROPERTY(EditAnywhere)
	FName handSocket;

	UPROPERTY(EditAnywhere)
	FName offHandSocket;

	UPROPERTY(EditAnywhere)
	FName offHandAttachSocket;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* weaponBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* traceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* traceEnd;

	UPROPERTY()
	bool collisionEnabled;

	UFUNCTION()
	void CreateOffHandWeapon(class AMainCharacter* player, USceneComponent* InParent);

	UFUNCTION()
	void CreateOffHandWeapon_Enemy(class AEnemy* enemy, USceneComponent* InParent);

};
