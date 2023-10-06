// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Character/MainCharacter.h"

AWeapon::AWeapon()
{
	handSocket = FName("RightHandSocket");
	attachSocket = FName("SwordHipSocket");
	combatType = ECombatTypes::LightSword;
	isAttached = false;
}

void AWeapon::Equip(USceneComponent* InParent)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, transformRules, handSocket);
	state = EItemState::Equipped;

	AMainCharacter* player = Cast<AMainCharacter>(InParent->GetOwner());
	if (player)
	{
		player->SetCombatState(combatType);
	}

	isAttached = true;
}

void AWeapon::UnEquip(USceneComponent* InParent)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, transformRules, attachSocket);
	state = EItemState::Unequipped;

	AMainCharacter* player = Cast<AMainCharacter>(InParent->GetOwner());
	if (player)
	{
		player->SetCombatState(ECombatTypes::None);
	}
}

void AWeapon::DropWeapon()
{
	FDetachmentTransformRules transformRules(EDetachmentRule::KeepWorld, true);
	ItemMesh->DetachFromComponent(transformRules);
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	state = EItemState::Hovering;
	isAttached = false;
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!isAttached)
		Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
