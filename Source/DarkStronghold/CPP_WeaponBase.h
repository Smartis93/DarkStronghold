// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_WeaponBase.generated.h"

// Forward Declarations
class UStaticMeshComponent;
class UCapsuleComponent;
class UCPP_DA_WeaponData;

UCLASS()
class DARKSTRONGHOLD_API ACPP_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_WeaponBase();

#if WITH_EDITOR
	// Chiamata nell'editor quando una proprietà viene modificata
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

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
