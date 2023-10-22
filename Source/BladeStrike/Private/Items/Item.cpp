// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/MainCharacter.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	itemFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Item FX"));
	itemFX->SetupAttachment(GetRootComponent());

	state = EItemState::Hovering;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}


void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* character = Cast<AMainCharacter>(OtherActor);
	if (character)
	{
		character->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainCharacter* character = Cast<AMainCharacter>(OtherActor);
	if (character)
	{
		character->SetOverlappingItem(nullptr);
	}
}



void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (state == EItemState::Hovering)
	{
		RunningTime += DeltaTime;
		ItemMesh->AddLocalOffset(FVector(0.f, 0.f, TransformedSin()));
		itemFX->AddLocalOffset(FVector(0.f, 0.f, -TransformedSin()));
		//AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));

		if (itemFX)
		{
			itemFX->SetVisibility(true);
			//itemFX->Activate();
		}
	}
	else
	{
		if (itemFX)
		{
			itemFX->SetVisibility(false);
			//itemFX->Deactivate();
		}
	}
}