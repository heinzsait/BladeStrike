// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnumClass.generated.h"


UCLASS()
class BLADESTRIKE_API UEnumClass : public UUserDefinedEnum
{
	GENERATED_BODY()


	
};


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Unequipped UMETA(DisplayName = "Unequipped"),
	Equipped UMETA(DisplayName = "Equipped Weapon"),
	Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ECharacterRotation : uint8
{
	Movement UMETA(DisplayName = "Movement"),
	Camera UMETA(DisplayName = "Camera")
};

UENUM(BlueprintType)
enum class ECombatTypes : uint8
{
	None UMETA(DisplayName = "None"),
	LightSword UMETA(DisplayName = "Light Sword"),
	HeavySword UMETA(DisplayName = "Heavy Sword"),
	DualSword UMETA(DisplayName = "Dual Sword")
};

UENUM(BlueprintType)
enum class ECharacterActions : uint8
{
	None UMETA(DisplayName = "None"),
	Attacking UMETA(DisplayName = "Attacking"),
	Dodging UMETA(DisplayName = "Dodging"),
	Blocking UMETA(DisplayName = "Blocking"),
	GotHit UMETA(DisplayName = "GotHit")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Hovering UMETA(DisplayName = "Hovering"),
	Equipped UMETA(DisplayName = "Equipped"),
	Unequipped UMETA(DisplayName = "Unequipped")
};

UENUM(BlueprintType)
enum class EAIBehaviors : uint8
{
	None UMETA(DisplayName = "None"),
	Patrol UMETA(DisplayName = "Patrol"),
	Chase UMETA(DisplayName = "Chase"),
	Attack UMETA(DisplayName = "Attack"),
	Hit UMETA(DisplayName = "Hit")
};

UENUM(BlueprintType)
enum class EAIAttackType : uint8
{
	Default UMETA(DisplayName = "Default"),
	CloseRange UMETA(DisplayName = "CloseRange"),
	MediumRange UMETA(DisplayName = "MediumRange"),
	LongRange UMETA(DisplayName = "LongRange")
};