// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkStronghold/CombatDefs.h"
#include "Engine/EngineTypes.h"
#include "CPP_AC_Combat.generated.h"

// Forward declarations
class UCPP_AC_InputBuffer;
class ACPP_WeaponBase;
class ACharacter;
class UAnimMontage;

/**
 * @class UCPP_AC_Combat
 * @brief Componente centrale che gestisce tutta la logica di combattimento di un personaggio.
 * * Gestisce gli stati del personaggio, l'esecuzione delle combo, la schivata e il rilevamento dei colpi.
 * Comunica con AC_InputBuffer per il timing degli input e legge i dati degli attacchi da un DA_WeaponData.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSTRONGHOLD_API UCPP_AC_Combat : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPP_AC_Combat();

	// --- Interfaccia Pubblica ----------------------------------------------------------------------------------------------
	
	/** Imposta un nuovo stato per il personaggio. */
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetCharacterState(ECharacterState NewState);

	/** Tenta di eseguire una schivata, controllando lo stato attuale e il cooldown. */
	void TryDodge();

	/** Funzione principale chiamata dal PlayerCharacter per processare l'input di attacco. */
	void HandleAttackInput(EAttackType AttackType);

	/** Attiva il tracciamento della hitbox dell'arma. Ideato per essere chiamato da un AnimNotifyState. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartWeaponTrace();

	/** Disattiva il tracciamento della hitbox dell'arma. Ideato per essere chiamato da un AnimNotifyState. */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopWeaponTrace();
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Funzione associata all'evento OnMontageEnded dell'AnimInstance per gestire le transizioni di stato. */
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// --- Proprietà Configurabili ----------------------------------------------------------------------------------------------

	/** L'animazione da riprodurre quando il personaggio schiva. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> DodgeMontage;

	/** La durata in secondi del cooldown della schivata. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge")
	float DodgeCooldown = 1.0f;
	
	// --- Riferimenti a Componenti ----------------------------------------------------------------------------------------------

	UPROPERTY()
	TObjectPtr<ACharacter> CharacterOwner;

	UPROPERTY()
	TObjectPtr<UCPP_AC_InputBuffer> InputBufferComponent;

private:
	// --- Stato Interno del Combattimento ----------------------------------------------------------------------------------------------
	
	/** Lo stato attuale del personaggio. */
	UPROPERTY(VisibleAnywhere, Category = "State")
	ECharacterState CurrentState = ECharacterState::Idle;

	/** L'indice dell'attacco successivo atteso nella catena di combo. */
	UPROPERTY(VisibleAnywhere, Category = "State")
	int32 ComboIndex = 0;

	/** Il nome della combo attualmente in esecuzione. */
	FName ActiveComboName;

	/** Un array di tutte le combo che sono ancora possibili in base agli input ricevuti. */
	TArray<const FComboData*> PotentialCombos;

	/** Puntatore ai dati dell'attacco attualmente in esecuzione. */
	const FSingleAttackData* CurrentAttackData = nullptr;

	/** Bool per il Trace della hitbox dell'arma. Viene cambiato da gli AnimNotifyState. */
	bool bIsTracingHitbox = false;

	/** Un elenco degli attori già colpiti durante un singolo attacco. Che poi verra usato per non dare più damage allo stesso Actor. */
	TArray<TObjectPtr<AActor>> HitActorsInSwing;
	
	// --- Logica Interna (Dodge) ------------------------------------------------------------------------------------------------------------------

	/** Timer utilizzato per gestire il cooldown della schivata. */
	FTimerHandle DodgeCooldownTimerHandle;

	/** Bool per sapere se la schivata è attualmente in cooldown. */
	bool bIsDodgeOnCooldown = false;

	/** Funzione chiamata per resettare il timer del cooldown della schivata. */
	void ResetDodgeCooldown();

	// --- Logica Interna (Combo) ------------------------------------------------------------------------------------------------------------------
	
	/** Funzione che resetta lo stato della combo e riporta il personaggio a Idle. */
	void ResetCombo(); //TODO. Sinceramente è da vedere se è l'ideale forzarlo ad idle da qui.

	/** Funzione che gestisce la logica per continuare una combo esistente in base al tipo di attacco. */
	void ContinueCombo(EAttackType BufferedAttackType);
};