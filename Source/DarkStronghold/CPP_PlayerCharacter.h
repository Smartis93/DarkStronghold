// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "CPP_PlayerCharacter.generated.h"

// Forward declaration
class UInputMappingContext;
class UInputAction;
class UCPP_AC_InputBuffer;
class UCPP_AC_Combat;
class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class ACPP_WeaponBase;

/**
 * @class ACPP_PlayerCharacter
 * @brief La classe base C++ per il personaggio giocante.
 *
 * Gestisce la creazione di tutti i componenti necessari (movimento, camera, combattimento),
 * il binding degli input tramite l'Enhanced Input System e la logica di interazione con
 * gli oggetti nel mondo. Delega le logiche complesse ai suoi Actor Components.
 */
UCLASS()
class DARKSTRONGHOLD_API ACPP_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_PlayerCharacter();

	/** Imposta l'arma equipaggiata e la memorizza nella variabile CurrentWeapon. */
	void SetEquippedWeapon(ACPP_WeaponBase* NewWeapon);

	/** Getter all'arma attualmente equipaggiata. */
	UFUNCTION(BlueprintPure, Category = "Combat")
	ACPP_WeaponBase* GetCurrentWeapon() const;

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- Input ---

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void LightAttack();
	void HeavyAttack();
	void Dodge();
	void DropWeapon();
	
	// --- Eventi ---

	/** Funzione chiamata quando un oggetto entra nel trigger di interazione. */
	UFUNCTION()
	void OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Funzione chiamata quando un oggetto esce dal trigger di interazione. */
	UFUNCTION()
	void OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// --- Componenti ---
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	TObjectPtr<UCPP_AC_InputBuffer> InputBufferComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	TObjectPtr<UCPP_AC_Combat> CombatComponent;

	// --- Camera ---
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UCameraComponent> ThirdPersonCameraComponent;

	// --- Interazione ---

	/** La zona sferica usata per rilevare oggetti con cui interagire. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<USphereComponent> InteractionTrigger;

	// --- Proprietà di Input ---

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	TObjectPtr<UInputMappingContext> PlayerMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	TObjectPtr<UInputAction> LightAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	TObjectPtr<UInputAction> HeavyAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	TObjectPtr<UInputAction> DropAction;
	
private:
	
	/** Un puntatore all'oggetto con cui possiamo attualmente interagire. */
	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<AActor> OverlappedInteractable;
	
	/** L'arma attualmente equipaggiata dal giocatore. */
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<ACPP_WeaponBase> CurrentWeapon;
};
