// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "CPP_DA_WeaponData.h"

// Sets default values
ACPP_WeaponBase::ACPP_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
	PickupTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Pickup Trigger"));
	PickupTrigger->SetupAttachment(RootComponent); //Buona pratica attaccarlo al root e non al mesh, non si sa mai se viene cambiato il root

}

#if WITH_EDITOR
void ACPP_WeaponBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Chiama sempre la funzione della classe base per prima cosa
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Controlla se la proprietà modificata è il nostro Data Asset
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ACPP_WeaponBase, WeaponDataAsset))
	{
		DrawDefaultHitboxPreview();
	}
}
#endif

void ACPP_WeaponBase::DrawDefaultHitboxPreview()
{
	// Se l'arma è in un mondo di gioco, non disegnare nulla.
	// Questo assicura che il disegno avvenga solo nell'editor.
	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		return;
	}
	
	// Cancella le vecchie linee di debug persistenti associate a questo attore
	FlushPersistentDebugLines(GetWorld());

	if (WeaponDataAsset && WeaponMesh)
	{
		for (const FHitboxTraceData& Hitbox : WeaponDataAsset->DefaultHitboxTraces)
		{
			const FVector Start = WeaponMesh->GetSocketLocation(Hitbox.StartSocket);
			const FVector End = WeaponMesh->GetSocketLocation(Hitbox.EndSocket);
			
			// Disegna una capsula persistente (durata -1)
			DrawDebugCapsule(GetWorld(), (Start + End) / 2.0f, (Start - End).Size() / 2.0f + Hitbox.Radius, Hitbox.Radius, 
							 FQuat::FindBetweenNormals(FVector::UpVector, (End - Start).GetSafeNormal()), 
							 FColor::Red, true, -1.0f, 0, 1.0f);
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

