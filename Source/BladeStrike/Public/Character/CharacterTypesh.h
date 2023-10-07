#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Unequipped UMETA(DisplayName = "Unequipped"),
	Equipped UMETA(DisplayName = "Equipped Weapon")
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
	LightAttack UMETA(DisplayName = "Light Attack"),
	HeavyAttack UMETA(DisplayName = "Heavy Attack"),
	ChargedAttack UMETA(DisplayName = "Charged Attack"),
	FallingAttack UMETA(DisplayName = "Falling Attack"),
	SpecialAttack UMETA(DisplayName = "Special Attack"),
	SprintingAttack UMETA(DisplayName = "Sprinting Attack")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Hovering,
	Equipped,
	Unequipped
};