#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Unequipped UMETA(DisplayName = "Unequipped"),
	Equipped UMETA(DisplayName = "Equipped Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	Free UMETA(DisplayName = "Free"),
	Attacking1 UMETA(DisplayName = "Attacking1"),
	CanAttack2 UMETA(DisplayName = "CanAttack2"),
	Attacking2 UMETA(DisplayName = "Attacking2"),
	CanAttack3 UMETA(DisplayName = "CanAttack3"),
	Attacking3 UMETA(DisplayName = "Attacking3")
};

UENUM(BlueprintType)
enum class ECombatTypes : uint8
{
	None UMETA(DisplayName = "None"),
	LightSword UMETA(DisplayName = "LightSword"),
	HeavySword UMETA(DisplayName = "HeavySword")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Hovering,
	Equipped,
	Unequipped
};