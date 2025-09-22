#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // Necessario per poter usare le struct nei Data Asset e Data Table
#include "CombatDefs.generated.h"

//====================================================================================
// ENUMS
//====================================================================================

/** Tipi di Attacco */
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None			UMETA(DisplayName = "None"),
	LightAttack		UMETA(DisplayName = "Light Attack"),
	HeavyAttack		UMETA(DisplayName = "Heavy Attack")
};

/** Stati del Personaggio */ //TODO. Da ampliare in futuro. (Attacchi Aerei?)
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Attacking   UMETA(DisplayName = "Attacking"),
	Dodging     UMETA(DisplayName = "Dodging")
};

//====================================================================================
// STRUCTS
//====================================================================================

/**
 * Struct per le hitbox, così nessuno può sbagliare
 */
USTRUCT(BlueprintType)
struct FHitboxTraceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StartSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EndSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1.0"))
	float Radius = 10.0f;
};

/**
 * Struct della composisioze del singolo attacco
 */
USTRUCT(BlueprintType)
struct FSingleAttackData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackType AttackType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	//TODO. Non so se mai mi serve, quindi essendo un override non rompo niente, l'importante è quello base.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(DisplayName="Override Hitboxes"))
	TArray<FHitboxTraceData> OverrideHitboxes;
};


/**
 * Struct delle Combo per il DataAsset
 */
USTRUCT(BlueprintType)
struct FComboData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ComboName;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSingleAttackData> AttackSequence;
};

