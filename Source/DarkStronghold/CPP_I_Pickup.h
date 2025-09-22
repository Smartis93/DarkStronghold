// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_I_Pickup.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_I_Pickup : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSTRONGHOLD_API ICPP_I_Pickup
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup")
	void Pickup(ACharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pickup")
	void Drop(); 
};
