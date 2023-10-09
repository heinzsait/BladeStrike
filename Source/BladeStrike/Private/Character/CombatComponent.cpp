// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CombatComponent.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//canAttack = true;
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
}

void UCombatComponent::ResetWeapon()
{
	if(mainWeapon)
		mainWeapon->attackIndex = 0;
}

void UCombatComponent::PerformAttack()
{
	UAnimMontage* _attackMontage = nullptr;
	if (!(character->GetCharacterMovement()->IsFalling() || character->GetCharacterMovement()->IsFlying())) // Character on ground
	{
		if(!character->isSprinting)
			_attackMontage = mainWeapon->attackMontages[mainWeapon->attackIndex];
		else
			_attackMontage = mainWeapon->sprintAttackMontages[mainWeapon->attackIndex];
	}
	else // Character in air
	{
		if (!character->isSprinting)
			_attackMontage = mainWeapon->fallingAttackMontages[mainWeapon->attackIndex];
		else 
			_attackMontage = mainWeapon->sprintJumpAttackMontage;
	}


	if (animInstance && _attackMontage)
	{
		if (!animInstance->Montage_IsPlaying(_attackMontage))
		{
			animInstance->Montage_Play(_attackMontage);

			character->SetCharacterActionState(ECharacterActions::Attacking);
		}
	}
}

void UCombatComponent::SelectNextAttack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, FString("selecting next montage"));

	if ((mainWeapon->attackMontages.Num() - 1) > mainWeapon->attackIndex)
		mainWeapon->attackIndex++;
	else
		mainWeapon->attackIndex = 0;
}

void UCombatComponent::PerformDodge()
{
	UAnimMontage* _dodgeMontage = nullptr;
	if (character->GetCharacterState() == ECharacterState::Equipped)
	{
		if (!(character->GetCharacterMovement()->IsFalling() || character->GetCharacterMovement()->IsFlying())) // Character on ground
		{
			_dodgeMontage = mainWeapon->dodgeMontage;
		}
	}
	else
	{
		_dodgeMontage = unarmedDodgeMontage;
	}


	if (animInstance && _dodgeMontage)
	{
		if (!animInstance->Montage_IsPlaying(_dodgeMontage))
		{
			animInstance->Montage_Play(_dodgeMontage);
			character->SetCharacterActionState(ECharacterActions::Dodging);
		}
	}
}

FRotator UCombatComponent::GetDodgeDirection()
{
	if (!character) return FRotator::ZeroRotator;
	FRotator targetRot = character->GetActorRotation();
	if (character->GetCharacterMovement())
	{
		if (!character->GetCharacterMovement()->GetLastInputVector().Equals(FVector::ZeroVector, 0.001f))
		{
			targetRot = UKismetMathLibrary::Conv_VectorToRotator(character->GetLastMovementInputVector());
		}
	}

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString("dodge dir = " + targetRot.ToString()));*/

	return targetRot;
}