// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

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

	if (ItemMesh && itemFX)
	{
		itemFX->SetVariableStaticMesh(TEXT("ItemMesh"), ItemMesh->GetStaticMesh());

		ShowItemVFX();
	}
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
	//if (state == EItemState::Hovering)
	//{
	//	//RunningTime += DeltaTime;
	//	//ItemMesh->AddLocalOffset(FVector(0.f, 0.f, TransformedSin()));
	//	//itemFX->AddLocalOffset(FVector(0.f, 0.f, -TransformedSin()));
	//	//AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));

	//	if (itemFX && ItemMesh)
	//	{
	//		itemFX->SetVisibility(true);
	//		ItemMesh->SetVisibility(false);
	//	}
	//}
	//else
	//{ 
	//	if (itemFX && ItemMesh)
	//	{
	//		itemFX->SetVisibility(false);
	//		ItemMesh->SetVisibility(true);
	//	}
	//}
}



void AItem::ShowItemVFX()
{
	if (itemFX && ItemMesh)
	{
		itemFX->SetVisibility(true);
		ItemMesh->SetVisibility(false);

		TArray<AActor*> actorsToIgnore;
		actorsToIgnore.Add(this);
		FHitResult hitResult;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Reserve(1);
		ObjectTypesArray.Emplace(ECollisionChannel::ECC_WorldStatic);

		UKismetSystemLibrary::SphereTraceSingleForObjects(this, ItemMesh->GetComponentLocation(), ItemMesh->GetComponentLocation() - FVector(0.0f, 0.0f, 500.0f), 10.0f, ObjectTypesArray, false, actorsToIgnore, EDrawDebugTrace::None, hitResult, true);

		if (hitResult.GetActor())
		{
			SetActorLocation(hitResult.ImpactPoint + FVector(0.0f, 0.0f, 50.0f));

			/*if (GetWorld()) 
				DrawDebugPoint(GetWorld(), hitResult.ImpactPoint, 15.f, FColor::Red, true, 10.0f);*/

			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString("Impact Point = " + hitResult.ImpactPoint.ToString()));
			}*/
		}
	}
}

void AItem::HideItemVFX()
{
	if (itemFX && ItemMesh)
	{
		itemFX->SetVisibility(false);
		ItemMesh->SetVisibility(true);
	}
}