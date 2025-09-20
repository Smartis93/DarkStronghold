// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponBase.h"

// Sets default values
ACPP_WeaponBase::ACPP_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
	PickupTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Pickup Trigger"));
	PickupTrigger->SetupAttachment(WeaponMesh);

}

void ACPP_WeaponBase::DrawDefaultHitboxPreview()
{
	if (WeaponDataAsset && WeaponMesh)
	{
		for (const FHitboxTraceData& Hitbox : WeaponDataAsset->DefaultHitboxTraces)
		{
			const FVector Start = WeaponMesh->GetSocketLocation(Hitbox.StartSocket);
			const FVector End = WeaponMesh->GetSocketLocation(Hitbox.EndSocket);
            
			// Calcola l'altezza della capsula
			const float HalfHeight = (Start - End).Size() / 2.0f;

			// Calcola il centro della capsula
			const FVector Center = (Start + End) / 2.0f;

			// Calcola la rotazione della capsula
			const FQuat Rotation = FRotationMatrix::MakeFromZ((End - Start).GetSafeNormal()).ToQuat();

			DrawDebugCapsule(GetWorld(), Center, HalfHeight, Hitbox.Radius, Rotation, FColor::Red, false, 5.0f);
		}
	}
}

// Called when the game starts or when spawned
void ACPP_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

