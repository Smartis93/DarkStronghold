// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_I_Damageable.h"
#include "CPP_Dummy.generated.h"

UCLASS()
class DARKSTRONGHOLD_API ACPP_Dummy : public ACharacter, public ICPP_I_Damageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Dummy();

	virtual void ApplyDamage_Implementation(float DamageAmount, const FHitResult& HitResult) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
