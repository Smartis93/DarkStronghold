// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_AC_InputBuffer.generated.h"


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

	/** Mette a false il bool AreBuffered. */
	UFUNCTION(BlueprintCallable, Category = "Input Buffer")
	void ClearBufferInput();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Se true, l'input può essere memorizzato nel buffer. Controllato da Open/CloseBufferWindow. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input Buffer", meta = (AllowPrivateAccess = "true"))
	bool bCanBuffer = false;

	/** Se true, è stato bufferizzato un input. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input Buffer", meta = (AllowPrivateAccess = "true"))
	bool bIsInputBuffered = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
