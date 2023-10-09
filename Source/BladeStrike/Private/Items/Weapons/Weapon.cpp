// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Character/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"

AWeapon::AWeapon()
{
	handSocket = FName("RightHandSocket");
	attachSocket = FName("SwordHipSocket");
	combatType = ECombatTypes::LightSword;
	isAttached = false;

	weaponBox = CreateDefaultSubobject<UBoxComponent>(FName("Weapon Box"));
	weaponBox->SetupAttachment(GetRootComponent());
	weaponBox->SetBoxExtent(FVector(3.0f, 2.0f, 40.0f));
	weaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	weaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	weaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	traceStart = CreateDefaultSubobject<USceneComponent>(FName("Weapon Box Trace Start"));
	traceStart->SetupAttachment(GetRootComponent());

	traceEnd = CreateDefaultSubobject<USceneComponent>(FName("Weapon Box Trace End"));
	traceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	weaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
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

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!collisionEnabled) return;

	const FVector start = traceStart->GetComponentLocation();
	const FVector end = traceEnd->GetComponentLocation();

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);
	for (AActor* actor : ignoreActors)
	{
		actorsToIgnore.AddUnique(actor);
	}
	FHitResult hitResult;

	UKismetSystemLibrary::BoxTraceSingle(this, start, end, FVector::OneVector * 5.0f, traceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, actorsToIgnore, EDrawDebugTrace::ForDuration, hitResult, true);

	if (hitResult.GetActor())
	{
		IHitInterface* hitInterface = Cast<IHitInterface>(hitResult.GetActor());
		if (hitInterface)
		{
			hitInterface->GetHit(hitResult.ImpactPoint);
		}
		ignoreActors.AddUnique(hitResult.GetActor());
	}
}
