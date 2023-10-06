// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CombatComponent.h"
#include "Character/MainCharacter.h"
#include "Items/Weapons/Weapon.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	canAttack = true;
	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	character = Cast<AMainCharacter>(GetOwner());
	if (character)
	{
		animInstance = character->GetMesh()->GetAnimInstance();
	}
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (animInstance)
	{
		//if (!animInstance->IsAnyMontagePlaying())
			//mainWeapon->attackIndex = 0;
	}

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Purple, FString::Printf(TEXT("can attack = %d"), canAttack));*/
}

void UCombatComponent::ResetWeapon()
{
	if(mainWeapon)
		mainWeapon->attackIndex = 0;
}

void UCombatComponent::PerformAttack()
{
	UAnimMontage* _attackMontage = mainWeapon->attackMontages[mainWeapon->attackIndex];

	if (animInstance && _attackMontage)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("playing montage index = %d"), mainWeapon->attackIndex));

		if (!animInstance->Montage_IsPlaying(_attackMontage))
		{
			animInstance->Montage_Play(_attackMontage);
			canAttack = false;
		}
		else
		{
			if ((mainWeapon->attackMontages.Num() - 1) > mainWeapon->attackIndex)
				mainWeapon->attackIndex++;
			else
				mainWeapon->attackIndex = 0;

			_attackMontage = mainWeapon->attackMontages[mainWeapon->attackIndex];
			if (_attackMontage)
			{
				animInstance->Montage_Play(_attackMontage);
				canAttack = false;
			}
		}
	}
}

