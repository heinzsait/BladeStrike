// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/StateManagerComponent.h"
#include "EnumClass.h"
#include "Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameManager.h"

// Sets default values for this component's properties
UStateManagerComponent::UStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//OnStateEnterDelegate.BindUFunction(this, FName("OnStateBegin"));
	//OnStateEndDelegate.BindUFunction(this, FName("OnStateEnded"));

	OnActionStateEnterDelegate.BindDynamic(this, &UStateManagerComponent::OnCharActionStateBegin);
	OnActionStateEndDelegate.BindDynamic(this, &UStateManagerComponent::OnCharActionStateEnded);

	OnCharStateEnterDelegate.BindDynamic(this, &UStateManagerComponent::OnCharStateBegin);
	OnCharStateEndDelegate.BindDynamic(this, &UStateManagerComponent::OnCharStateEnded);

	rotationState = ECharacterRotation::Movement;
	character = Cast<AMainCharacter>(GetOwner());
	
	GM = GetWorld()->GetSubsystem<UGameManager>();
}


// Called every frame
void UStateManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CharActionStateUpdate();
}

ECharacterState UStateManagerComponent::GetCurrentCharacterState()
{
	return currentCharacterState;
}

void UStateManagerComponent::SetCurrentCharacterState(ECharacterState state)
{
	if (currentCharacterState != state)
	{
		OnCharStateEndDelegate.Execute(state);
		currentCharacterState = state;
		OnCharStateEnterDelegate.Execute(currentCharacterState);
	}
}

ECharacterRotation UStateManagerComponent::GetCharacterRotationState()
{
	return rotationState;
}

void UStateManagerComponent::SetCharacterRotationState(ECharacterRotation state)
{
	rotationState = state;
	UpdateCharacterRotation();
}

void UStateManagerComponent::UpdateCharacterRotation()
{
	switch (rotationState)
	{
	case ECharacterRotation::Camera:
		character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		character->GetCharacterMovement()->bOrientRotationToMovement = false;
		break;

	case ECharacterRotation::Movement:
		character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		character->GetCharacterMovement()->bOrientRotationToMovement = true;

		break;
	default:
		break;
	}
}

void UStateManagerComponent::ResetState()
{

}

ECharacterActions UStateManagerComponent::GetCharacterActionState()
{
	return characterActionState;
}

void UStateManagerComponent::SetCharacterActionState(ECharacterActions state)
{
	if (characterActionState != state)
	{
		OnActionStateEndDelegate.Execute(characterActionState);
		characterActionState = state;
		OnActionStateEnterDelegate.Execute(state);
	}
}

void UStateManagerComponent::OnCharStateBegin(const ECharacterState state)
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Red, FString("Entering state: " + int(state)));*/
}

void UStateManagerComponent::OnCharStateEnded(const ECharacterState state)
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Red, FString("Exiting state: " + int(state)));*/
}

void UStateManagerComponent::OnCharActionStateBegin(const ECharacterActions state)
{
	switch (state)
	{
	case ECharacterActions::None:

		break;

	case ECharacterActions::Attacking:

		if (GM)
			GM->SetPlayerAttacking(true);
	
		break;

	case ECharacterActions::Dodging:
		character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		character->GetCharacterMovement()->bOrientRotationToMovement = true;
		break;

	case ECharacterActions::GotHit:

		break;

	default:
		break;
	}
}

void UStateManagerComponent::OnCharActionStateEnded(const ECharacterActions state)
{
	switch (state)
	{
	case ECharacterActions::None:

		break;

	case ECharacterActions::Attacking:
		character->GetCombatComponent()->SelectNextAttack();

		if (GM)
			GM->SetPlayerAttacking(false);

		break;

	case ECharacterActions::Dodging:
		UpdateCharacterRotation();
		break;

	case ECharacterActions::GotHit:

		break;

	default:
		break;
	}
}

void UStateManagerComponent::CharActionStateUpdate()
{
	switch (characterActionState)
	{
	case ECharacterActions::None:

		break;

	case ECharacterActions::Attacking:

		break;

	case ECharacterActions::Dodging:

		break;

	case ECharacterActions::GotHit:

		break;

	default:
		break;
	}
}

