// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLADESTRIKE_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TargetedEnemyDied(AActor* enemy);

	UFUNCTION()
	AActor* FindTarget();

	UFUNCTION()
	void LockTarget();

	FORCEINLINE bool IsLocked() const { return isLocked; }

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* lockOnUI;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	UPROPERTY()
	class AMainCharacter* character;

	UPROPERTY()
	class UCameraComponent* camera;

	UPROPERTY()
	bool isLocked = false;

	UPROPERTY()
	AActor* lockedOnTarget;

	UFUNCTION()
	void UpdateTargetRotation();
		
};
