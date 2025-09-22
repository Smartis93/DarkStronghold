// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CPP_DA_WeaponData.h"

// Sets default values
ACPP_WeaponBase::ACPP_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;

	PickupTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Pickup Trigger"));
	PickupTrigger->SetupAttachment(RootComponent); //Da quanto ho capito è buona pratica attaccarlo al root e non al mesh, non si sa mai se viene cambiato il root

}

void ACPP_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// --- Interfaccia ICPP_I_Pickup ---

void ACPP_WeaponBase::Pickup_Implementation(ACharacter* Character)
{
	if (!Character) return;
	
	if (PickupTrigger)
	{
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(false);
	}

	AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r_sword_socket"));
	SetOwner(Character);
}

void ACPP_WeaponBase::Drop_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (PickupTrigger)
	{
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	if (WeaponMesh)
	{
		WeaponMesh->SetSimulatePhysics(true);
	}
	
	SetOwner(nullptr);
}

// --- Editor Debug per le hitbox ---

#if WITH_EDITOR
void ACPP_WeaponBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACPP_WeaponBase, bDrawHitboxPreview))
	{
		if (bDrawHitboxPreview)
		{
			DrawDefaultHitboxPreview();
			bDrawHitboxPreview = false;
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(ACPP_WeaponBase, bClearHitboxPreview))
	{
		if (bClearHitboxPreview)
		{
			FlushPersistentDebugLines(GetWorld());
			bClearHitboxPreview = false;
		}
	}
}

void ACPP_WeaponBase::DrawDefaultHitboxPreview()
{
	if (GetWorld() && GetWorld()->IsGameWorld()) return;
	
	FlushPersistentDebugLines(GetWorld());
	
	if (!WeaponDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Draw Preview Failed: WeaponDataAsset non assegnato."));
		return;
	}

	if (!WeaponMesh || !WeaponMesh->GetStaticMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("Draw Preview Failed: WeaponMesh nullo o senza una mesh assegnata."));
		return;
	}

	if (WeaponDataAsset->DefaultHitboxTraces.IsEmpty())
	{
		 UE_LOG(LogTemp, Warning, TEXT("Nessuna hitbox da disegnare: l'array DefaultHitboxTraces è vuoto."));
		 return;
	}
	
	for (const FHitboxTraceData& Hitbox : WeaponDataAsset->DefaultHitboxTraces)
	{
		const FVector Start = WeaponMesh->GetSocketLocation(Hitbox.StartSocket);
		const FVector End = WeaponMesh->GetSocketLocation(Hitbox.EndSocket);
		
		DrawDebugCapsule(GetWorld(), (Start + End) / 2.0f, (Start - End).Size() / 2.0f + Hitbox.Radius, Hitbox.Radius, 
						 FQuat::FindBetweenNormals(FVector::UpVector, (End - Start).GetSafeNormal()), 
						 FColor::Red, true, -1.0f, 0, 1.0f);
	}
}
#endif