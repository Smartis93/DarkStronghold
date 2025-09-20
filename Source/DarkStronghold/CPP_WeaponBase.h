// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "CPP_DA_WeaponData.h"
#include "CPP_WeaponBase.generated.h"

UCLASS()
class DARKSTRONGHOLD_API ACPP_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_WeaponBase();

	/** Funzione per disegnare le hitbox di default direttamente nell'editor. */
	UFUNCTION(CallInEditor, Category = "Debug")
	void DrawDefaultHitboxPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	// Questo è il nostro "contratto": ogni arma avrà un riferimento a un moveset.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UCPP_DA_WeaponData> WeaponDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	TObjectPtr<UCapsuleComponent> PickupTrigger;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCPP_DA_WeaponData* GetWeaponDataAsset() const {return WeaponDataAsset;}
};
