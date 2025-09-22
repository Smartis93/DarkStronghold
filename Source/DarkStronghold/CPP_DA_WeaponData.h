// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DarkStronghold/CombatDefs.h"
#include "CPP_DA_WeaponData.generated.h"

/**
 * @class UCPP_DA_WeaponData
 * @brief Un Data Asset che contiene l'intero moveset di un'arma.
 * Questo asset funge da "database" per una singola arma, definendo tutte le sue combo e danni dei singoli attacchi
 */
UCLASS()
class DARKSTRONGHOLD_API UCPP_DA_WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/**
	 * @brief La configurazione delle hitbox di default per quest'arma.
	 * Vengono usate per qualsiasi attacco che non abbia un suo set di hitbox di override.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Data")
	TArray<FHitboxTraceData> DefaultHitboxTraces;
	
	/**
	 * @brief L'elenco di tutte le combo che quest'arma può eseguire.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat Data")
	TArray<FComboData> Combos;
};
