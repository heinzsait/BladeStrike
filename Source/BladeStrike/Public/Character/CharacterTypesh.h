#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Unequipped UMETA(DisplayName = "Unequipped"),
	Equipped UMETA(DisplayName = "Equipped Weapon")
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
	HeavySword UMETA(DisplayName = "Heavy Sword")
};

UENUM(BlueprintType)
enum class ECharacterActions : uint8
{
	None UMETA(DisplayName = "None"),
	Attacking UMETA(DisplayName = "Attacking"),
	Dodging UMETA(DisplayName = "Dodging")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Hovering,
	Equipped,
	Unequipped
};