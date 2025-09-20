// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DarkStronghold/CombatDefs.h"
#include "CPP_DA_WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class DARKSTRONGHOLD_API UCPP_DA_WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox Default")
	TArray<FHitboxTraceData> DefaultHitboxTraces;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Moveset Data")
	TArray<FComboData> Combos;
};
