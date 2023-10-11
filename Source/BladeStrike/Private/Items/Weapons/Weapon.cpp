// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Character/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

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
	weaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//weaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

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

	if (!isWeaponEnemy)
	{
		AMainCharacter* player = Cast<AMainCharacter>(InParent->GetOwner());
		if (player)
		{
			player->SetCombatState(combatType);
		}
	}

	SetOwner(InParent->GetOwner());
	SetInstigator(Cast<APawn>(InParent->GetOwner()));
	isAttached = true;
}

void AWeapon::UnEquip(USceneComponent* InParent)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, transformRules, attachSocket);
	state = EItemState::Unequipped;

	if (!isWeaponEnemy)
	{
		AMainCharacter* player = Cast<AMainCharacter>(InParent->GetOwner());
		if (player)
		{
			player->SetCombatState(ECombatTypes::None);
		}
	}
}

void AWeapon::DropWeapon()
{
	FDetachmentTransformRules transformRules(EDetachmentRule::KeepWorld, true);
	ItemMesh->DetachFromComponent(transformRules);
	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	state = EItemState::Hovering;
	isAttached = false;
	SetOwner(nullptr);
	SetInstigator(nullptr);
	isWeaponEnemy = false;
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
	
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("Detected Overlap")));*/

	const FVector start = traceStart->GetComponentLocation();
	const FVector end = traceEnd->GetComponentLocation();

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);
	actorsToIgnore.Add(GetOwner());
	for (AActor* actor : ignoreActors)
	{
		actorsToIgnore.AddUnique(actor);
	}
	FHitResult hitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Reserve(1);
	ObjectTypesArray.Emplace(ECollisionChannel::ECC_Pawn);

	UKismetSystemLibrary::SphereTraceSingleForObjects(this, start, end, 10.0f, ObjectTypesArray, false, actorsToIgnore, EDrawDebugTrace::None, hitResult, true);
	//UKismetSystemLibrary::BoxTraceSingle(this, start, end, FVector::OneVector * 5.0f, traceStart->GetComponentRotation(), ETraceTypeQuery::, false, actorsToIgnore, EDrawDebugTrace::None, hitResult, true);

	if (hitResult.GetActor())
	{
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), weaponDamage, GetInstigatorController(), this, UDamageType::StaticClass());

		IHitInterface* hitInterface = Cast<IHitInterface>(hitResult.GetActor());
		if (hitInterface)
		{
			hitInterface->GetHit(hitResult.ImpactPoint);
			ignoreActors.AddUnique(hitResult.GetActor());
		}
	}
}
