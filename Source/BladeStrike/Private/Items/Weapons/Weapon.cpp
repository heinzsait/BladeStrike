// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Character/MainCharacter.h"
#include "Enemy/Enemy.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	handSocket = FName("RightHandSocket");
	offHandSocket = FName("LeftHandSocket");
	
	attachSocket = FName("SwordHipSocket");
	offHandAttachSocket = FName("SwordHipSocket");

	combatType = ECombatTypes::LightSword;
	isAttached = false;

	weaponBox = CreateDefaultSubobject<UBoxComponent>(FName("Weapon Box"));
	weaponBox->SetupAttachment(GetRootComponent());
	weaponBox->SetBoxExtent(FVector(3.0f, 2.0f, 40.0f));
	weaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	weaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	weaponBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

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

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::Equip(USceneComponent* InParent)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	if(!isOffHanded)
		ItemMesh->AttachToComponent(InParent, transformRules, handSocket);
	else
		ItemMesh->AttachToComponent(InParent, transformRules, offHandSocket);


	state = EItemState::Equipped;
	SetOwner(InParent->GetOwner());
	SetInstigator(Cast<APawn>(InParent->GetOwner()));
	isAttached = true;

	if (!isWeaponEnemy)
	{
		AMainCharacter* player = Cast<AMainCharacter>(InParent->GetOwner());
		if (player)
		{
			player->SetCombatState(combatType);
			if (combatType == ECombatTypes::DualSword)
			{	
				if (!isOffHanded)
				{
					CreateOffHandWeapon(player, InParent);
				}
			}

			if (hasShield)
			{
				if (!isOffHanded)
				{
					CreateOffHandShield(player, InParent);
				}
			}
		}
	}
	else
	{
		if (combatType == ECombatTypes::DualSword)
		{
			AEnemy* enemy = Cast<AEnemy>(InParent->GetOwner());
			if (enemy)
			{
				if (!isOffHanded)
				{
					CreateOffHandWeapon_Enemy(enemy, InParent);
				}
			}
		}
	}
}



void AWeapon::UnEquip(USceneComponent* InParent)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	if(!isOffHanded)
		ItemMesh->AttachToComponent(InParent, transformRules, attachSocket);
	else
		ItemMesh->AttachToComponent(InParent, transformRules, offHandAttachSocket);
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

void AWeapon::CreateOffHandWeapon(AMainCharacter* player, USceneComponent* InParent)
{
	if (player->GetCombatComponent()->GetOffHandWeapon() == nullptr)
	{
		//Create copy of this weapon to off hand...
		AWeapon* offHandWeapon = Clone();

		offHandWeapon->isOffHanded = true;
		FAttachmentTransformRules _transformRules(EAttachmentRule::SnapToTarget, true);
		offHandWeapon->ItemMesh->AttachToComponent(InParent, _transformRules, offHandSocket);
		offHandWeapon->state = EItemState::Equipped;

		offHandWeapon->SetOwner(InParent->GetOwner());
		offHandWeapon->SetInstigator(Cast<APawn>(InParent->GetOwner()));
		offHandWeapon->isAttached = true;

		offHandPair = offHandWeapon;
		offHandWeapon->mainHandPair = this;

		player->GetCombatComponent()->SetOffHandWeapon(offHandWeapon);
	}
}

void AWeapon::CreateOffHandShield(AMainCharacter* player, USceneComponent* InParent)
{
	if (player->GetCombatComponent()->GetOffHandWeapon() == nullptr)
	{
		//Create copy of this weapon to off hand...
		AWeapon* offHandWeapon = CreateShield();

		offHandWeapon->isOffHanded = true;
		FAttachmentTransformRules _transformRules(EAttachmentRule::SnapToTarget, true);
		offHandWeapon->ItemMesh->AttachToComponent(InParent, _transformRules, offHandSocket);
		offHandWeapon->state = EItemState::Equipped;

		offHandWeapon->SetOwner(InParent->GetOwner());
		offHandWeapon->SetInstigator(Cast<APawn>(InParent->GetOwner()));
		offHandWeapon->isAttached = true;

		offHandPair = offHandWeapon;
		offHandWeapon->mainHandPair = this;

		player->GetCombatComponent()->SetOffHandWeapon(offHandWeapon);
	}
}

void AWeapon::CreateOffHandWeapon_Enemy(AEnemy* enemy, USceneComponent* InParent)
{
	if (enemy->offHandWeapon == nullptr)
	{
		//Create copy of this weapon to off hand...
		AWeapon* offHandWeapon = Clone();

		offHandWeapon->isOffHanded = true;
		FAttachmentTransformRules _transformRules(EAttachmentRule::SnapToTarget, true);
		offHandWeapon->ItemMesh->AttachToComponent(InParent, _transformRules, offHandSocket);
		offHandWeapon->state = EItemState::Equipped;

		offHandWeapon->SetOwner(InParent->GetOwner());
		offHandWeapon->SetInstigator(Cast<APawn>(InParent->GetOwner()));
		offHandWeapon->isAttached = true;
		offHandWeapon->isWeaponEnemy = true;

		offHandPair = offHandWeapon;
		offHandWeapon->mainHandPair = this;

		enemy->offHandWeapon = offHandWeapon;
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

	FHitResult hitResult;
	FCollisionQueryParams collisionParams;
	//collisionParams.
	ItemMesh->LineTraceComponent(hitResult, GetActorLocation() + FVector(0.0f, 0.0f, 300.0f), GetActorLocation() - FVector(0.0f, 0.0f, 300.0f), collisionParams);

	if (hitResult.GetActor())
	{
		SetActorLocation(hitResult.ImpactPoint);
	}

	offHandPair = nullptr;
}

AWeapon* AWeapon::Clone()
{
	FActorSpawnParameters Parameters;
	Parameters.Template = this;

	AWeapon* weapon = GetWorld()->SpawnActor<AWeapon>(GetClass(), Parameters);
	weapon->SetOwner(GetOwner());

	return weapon;
}

AWeapon* AWeapon::CreateShield()
{
	if (!shieldToSpawn) return nullptr;

	AWeapon* weapon = GetWorld()->SpawnActor<AWeapon>(shieldToSpawn);
	weapon->SetOwner(GetOwner());

	return weapon;
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
	if (isShield) return;
	
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
	
	if (hitResult.GetActor())
	{	
		IHitInterface* hitInterface = Cast<IHitInterface>(hitResult.GetActor());
		if (hitInterface)
		{
			if (isWeaponEnemy)
			{
				if (hitResult.GetActor()->ActorHasTag(FName("Player")))
				{
					AMainCharacter* player = Cast<AMainCharacter>(hitResult.GetActor());
					if (player)
					{
						if (player->GetCharacterActionState() != ECharacterActions::GotHit)
						{
							UGameplayStatics::ApplyDamage(player, weaponDamage, GetInstigatorController(), this, UDamageType::StaticClass());
							hitInterface->GetHit(hitResult.ImpactPoint);
							if (hitParticlesEnemy && GetWorld())
								UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitParticlesEnemy, hitResult.ImpactPoint);
						}
					}
				}
			}
			else
			{
				UGameplayStatics::ApplyDamage(hitResult.GetActor(), weaponDamage, GetInstigatorController(), this, UDamageType::StaticClass());
				hitInterface->GetHit(hitResult.ImpactPoint);
				if (hitParticles && GetWorld())
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitParticles, hitResult.ImpactPoint);
			}

			ignoreActors.AddUnique(hitResult.GetActor());
		}
	}
}
