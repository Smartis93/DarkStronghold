// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AC_Combat.h"
#include "GameFramework/Character.h"
#include "Darkstronghold/CPP_AC_InputBuffer.h" 
#include "Darkstronghold/CPP_WeaponBase.h" 
#include "Darkstronghold/CPP_PlayerCharacter.h"

// Sets default values for this component's properties
UCPP_AC_Combat::UCPP_AC_Combat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPP_AC_Combat::BeginPlay()
{
	Super::BeginPlay();

	// Ottieni un riferimento al proprietario e al suo input buffer all'avvio
	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		InputBufferComponent = CharacterOwner->FindComponentByClass<UCPP_AC_InputBuffer>();
	}
	
}


// Called every frame
void UCPP_AC_Combat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!InputBufferComponent || !CharacterOwner) return;

	// Possiamo iniziare un attacco solo se siamo in stato di Idle
	if (CurrentState != ECharacterState::Idle) return;

	// Controlla se c'è un input nel buffer
	const EAttackType BufferedAttack = InputBufferComponent->GetBufferedAttackType();
	if (BufferedAttack != EAttackType::None)
	{
		// Se c'è un input, consumalo (pulendo il buffer) e prova ad eseguire l'attacco
		InputBufferComponent->ClearBufferInput();
		ExecuteAttack(BufferedAttack);
	}
}

void UCPP_AC_Combat::ExecuteAttack(EAttackType AttackType)
{
	// Per ora, gestiamo solo l'inizio di una combo
	if (ComboIndex != 0) return;

	// Ottieni l'arma equipaggiata dal player
	ACPP_PlayerCharacter* PlayerCharacter = Cast<ACPP_PlayerCharacter>(CharacterOwner);
	if (!PlayerCharacter) return;
	
	// TODO: La logica per ottenere l'arma corrente andrà qui. Per ora la saltiamo.
	// ACPP_WeaponBase* CurrentWeapon = PlayerCharacter->GetCurrentWeapon();
	// if (!CurrentWeapon) return;
	// UCPP_DA_WeaponData* WeaponData = CurrentWeapon->GetWeaponDataAsset();
	// if (!WeaponData) return;
	
	// --- LOGICA TEMPORANEA PER TEST ---
	UE_LOG(LogTemp, Log, TEXT("Executing Attack of type %s"), *UEnum::GetValueAsString(AttackType));
	// ------------------------------------

	// TODO: Cerca nel WeaponData una combo che inizia con questo AttackType.
	// Se la trovi:
	// 1. Prendi il primo FSingleAttackData dalla sequenza.
	// 2. CharacterOwner->PlayAnimMontage(AttackData->AnimMontage);
	// 3. CurrentState = ECharacterState::Attacking;
	// 4. ComboIndex = 1; // Avanza l'indice per il prossimo attacco della combo
}