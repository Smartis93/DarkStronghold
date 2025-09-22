// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_I_Pickup.h"
#include "CPP_WeaponBase.generated.h"

// Forward Declarations
class UStaticMeshComponent;
class UCapsuleComponent;
class UCPP_DA_WeaponData;

/**
 * @class ACPP_WeaponBase
 * @brief La classe base C++ per tutte le armi nel gioco.
 *
 * Questo actor rappresenta un'arma fisica nel mondo. Contiene tutti componenti di base (mesh, trigger),
 * implementa l'interfaccia di Pickup/Drop e contiene un riferimento a un Data Asset (UCPP_DA_WeaponData)
 */
UCLASS()
class DARKSTRONGHOLD_API ACPP_WeaponBase : public AActor, public ICPP_I_Pickup
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	ACPP_WeaponBase();

	/** Getter Data Asset del moveset associato a quest'arma. */
	UCPP_DA_WeaponData* GetWeaponDataAsset() const { return WeaponDataAsset; }

	// --- Interfaccia ICPP_I_Pickup ---
	
	virtual void Pickup_Implementation(ACharacter* Character) override;
	virtual void Drop_Implementation() override;

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// --- Componenti ---

	/** La mesh visibile dell'arma. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	/** Il trigger per il PickUp dell'arma. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> PickupTrigger;

	// --- Dati dell'arma ---
	
	/** Il Data Asset di quest'arma. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UCPP_DA_WeaponData> WeaponDataAsset;

	// --- Debug ---
	
#if WITH_EDITOR
	/** Funzione chiamata automaticamente dall'editor quando una proprietà di questo attore viene modificata. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	//Nota: Avevo provato con un bottone ma senza successo
	
	/** Mostra una preview visiva delle hitbox di default. */
	UPROPERTY(EditAnywhere, Category = "Debug", meta=(DisplayName="Show Hitbox Preview"))
	bool bDrawHitboxPreview;
	
	/** Pulisce la preview visiva delle hitbox. */
	UPROPERTY(EditAnywhere, Category = "Debug", meta=(DisplayName="Hide Hitbox Preview"))
	bool bClearHitboxPreview;

private:
	/** Debug Hitbox */
	void DrawDefaultHitboxPreview();
};