// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Dummy.h"

// Sets default values
ACPP_Dummy::ACPP_Dummy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_Dummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Dummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_Dummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_Dummy::ApplyDamage_Implementation(float DamageAmount, const FHitResult& HitResult)
{
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("%s took %.1f damage! Current health: %.1f"), *this->GetName(), DamageAmount, Health);

	if (Health <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has been defeated!"), *this->GetName());
		Destroy();
	}
}

