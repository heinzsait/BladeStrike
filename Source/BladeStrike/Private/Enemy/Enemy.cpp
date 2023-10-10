// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MainCharacter.h"
#include "AIController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(this, AMainCharacter::StaticClass()));

	/*if (player && ai)
	{
		ai->MoveToActor(player);
	}*/
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& impactPoint)
{
	DirectionalHitReact(impactPoint);


}

void AEnemy::DirectionalHitReact(const FVector& impactPoint)
{
	const FVector forward = GetActorForwardVector();
	const FVector _impactPoint = FVector(impactPoint.X, impactPoint.Y, GetActorLocation().Z);
	const FVector toHit = (_impactPoint - GetActorLocation()).GetSafeNormal();

	const double cosO = FVector::DotProduct(forward, toHit);
	double O = FMath::RadiansToDegrees(FMath::Acos(cosO));
	if (FVector::CrossProduct(forward, toHit).Z < 0)  O *= -1;

	FName section = FName("FromBack");
	if (O >= -45.f && O < 45.f)  section = FName("FromFront");
	else if (O >= -135.f && O < -45.f)  section = FName("FromLeft");
	else if (O >= 45.f && O < 135.f)  section = FName("FromRight");

	PlayHitReaction(section);
}

void AEnemy::PlayHitReaction(const FName sectionName)
{
	if (GetMesh()->GetAnimInstance() && hitReactionMontages)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(hitReactionMontages);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(sectionName);
	}
}


//void AEnemy::TakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
//{
//
//}

