// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkStronghold/CombatDefs.h"
#include "CPP_AC_Combat.generated.h"

// Forward declarations
class UCPP_AC_InputBuffer;
class ACPP_WeaponBase;
class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSTRONGHOLD_API UCPP_AC_Combat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPP_AC_Combat();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Variabili di Stato
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterState CurrentState = ECharacterState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	int32 ComboIndex = 0;

	// Riferimenti ad altri oggetti/componenti
	UPROPERTY()
	TObjectPtr<ACharacter> CharacterOwner;

	UPROPERTY()
	TObjectPtr<UCPP_AC_InputBuffer> InputBufferComponent;

	// Funzione interna per avviare un attacco
	void ExecuteAttack(EAttackType AttackType);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
