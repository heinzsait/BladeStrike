// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class BLADESTRIKE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	UFUNCTION()
	void Equip(USceneComponent* InParent);

	UFUNCTION()
	void UnEquip(USceneComponent* InParent);

	UFUNCTION()
	void DropWeapon();

	UPROPERTY(EditAnywhere)
	FName attachSocket;

	UPROPERTY(EditAnywhere)
	FName handSocket;

	UPROPERTY(EditAnywhere)
	UAnimMontage* swordDrawMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* swordDisarmMontage;

	UPROPERTY(EditAnywhere)
	ECombatTypes combatType;

	UPROPERTY()
	bool canInteract = true;

	UPROPERTY(EditAnywhere)
	UAnimMontage* attackMontage;
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
