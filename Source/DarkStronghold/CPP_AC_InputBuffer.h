// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkStronghold/CombatDefs.h"
#include "CPP_AC_InputBuffer.generated.h"

/**
 * @class UCPP_AC_InputBuffer
 * @brief Gestisce la logica di per la memorizzazione degli input di attacco.
 * Questo componente non memorizza input in modo indiscriminato,
 * ma viene gestita da bCanBuffer che decide quando un input di attacco può essere registrato.
 * Questa "finestra temporale" viene tipicamente aperta e chiusa da Anim Notify State durante le animazioni di attacco.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSTRONGHOLD_API UCPP_AC_InputBuffer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPP_AC_InputBuffer();

	/** Apre la finestra temporale in cui è possibile bufferizzare un input. */
	UFUNCTION(BlueprintCallable, Category = "Input Buffer")
	void OpenBufferWindow();

	/** Chiude la finestra temporale in cui è possibile bufferizzare un input. */
	UFUNCTION(BlueprintCallable, Category = "Input Buffer")
	void CloseBufferWindow();

	/** Pulisce l'input attualmente bufferizzato. */
	UFUNCTION(BlueprintCallable, Category = "Input Buffer")
	void ClearBufferInput();

	/** Il cervello del componente che gestisce se può o non può */
	void TryBufferAttack(EAttackType AttackType);

	/** Getter del tipo di attacco attualmente bufferizzato. */
	UFUNCTION(BlueprintPure, Category = "Input Buffer")
	EAttackType GetBufferedAttackType() const;

protected:
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/** Il bool che gestisce l'apertura, ma rimane privata e se ne occupano delle funzioni pubbliche a cambiarla */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input Buffer", meta = (AllowPrivateAccess = "true"))
	bool bCanBuffer = false;

	/** Un bool che gestisce se un input di attacco è in attesa. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input Buffer", meta = (AllowPrivateAccess = "true"))
	bool bIsInputBuffered = false;

	/** Il tipo di attacco attualmente memorizzato nel buffer. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input Buffer", meta = (AllowPrivateAccess = "true"))
	EAttackType BufferedAttackType = EAttackType::None;
};
