#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // Necessario per poter usare le struct nei Data Asset e Data Table
#include "CombatDefs.generated.h"

// Enum per i tipi di attacco
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None UMETA(DisplayName = "None"), //in realtà non è necessario, TODO capire se serve realmente o no.
	LightAttack UMETA(DisplayName = "Light Attack"),
	HeavyAttack UMETA(DisplayName = "Heavy Attack")
};

// Enum per gli stati del personaggio
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Attacking   UMETA(DisplayName = "Attacking"),
	Dodging     UMETA(DisplayName = "Dodging")
};

//-------------------------------------------------------------------
// STRUCTS PER IL DATA ASSET DELLE ARMI
//-------------------------------------------------------------------

/**
 * @brief Definisce i dati per un singolo trace di hitbox (una capsula).
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
	float Radius = 10.0f; // Raggio di default di 10cm
};

/**
 * @brief Definisce i dati per un singolo attacco all'interno di una combo.
 */
USTRUCT(BlueprintType)
struct FSingleAttackData
{
	GENERATED_BODY()

	// Il tipo di input richiesto per eseguire questo attacco
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackType AttackType;

	// L'animazione da riprodurre per questo attacco
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AnimMontage;

	// Il danno base di questo attacco
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	/** Se questo array non è vuoto, sovrascrive la hitbox di default dell'arma. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta=(DisplayName="Override Hitboxes"))
	TArray<FHitboxTraceData> OverrideHitboxes;
};


/**
 * @brief Definisce un'intera sequenza di combo.
 */
USTRUCT(BlueprintType)
struct FComboData
{
	GENERATED_BODY()

	// Il nome identificativo della combo (es. "Light Combo 1")
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ComboName;
    
	// La sequenza di attacchi che compongono questa combo
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSingleAttackData> AttackSequence;
};

