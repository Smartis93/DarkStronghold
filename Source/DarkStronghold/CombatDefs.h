#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // Necessario per poter usare le struct nei Data Asset e Data Table
#include "CombatDefs.generated.h"

// Enum per i tipi di attacco
UENUM(BlueprintType)
enum class EAttackType : uint8
{
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
 * @brief Definisce i dati per un singolo attacco all'interno di una combo.
 */
USTRUCT(BlueprintType)
struct FSingleAttackData
{
	GENERATED_BODY()

	// Il tipo di input richiesto per eseguire questo attacco [cite: 142]
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackType AttackType;

	// L'animazione da riprodurre per questo attacco [cite: 143]
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AnimMontage;

	// Il danno base di questo attacco [cite: 144]
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;
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
    
	// La sequenza di attacchi che compongono questa combo [cite: 139]
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSingleAttackData> AttackSequence;
};