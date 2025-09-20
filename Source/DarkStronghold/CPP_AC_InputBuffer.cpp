// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AC_InputBuffer.h"

// Sets default values for this component's properties
UCPP_AC_InputBuffer::UCPP_AC_InputBuffer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	BufferedAttackType = EAttackType::None;

}


// Called when the game starts
void UCPP_AC_InputBuffer::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCPP_AC_InputBuffer::OpenBufferWindow()
{
	bCanBuffer = true;
	UE_LOG(LogTemp, Log, TEXT("Input Buffer Window: OPEN"));
}

void UCPP_AC_InputBuffer::CloseBufferWindow()
{
	bCanBuffer = false;
	UE_LOG(LogTemp, Log, TEXT("Input Buffer Window: CLOSED"));
}

void UCPP_AC_InputBuffer::ClearBufferInput()
{
	bIsInputBuffered = false;
	BufferedAttackType = EAttackType::None;
	UE_LOG(LogTemp, Log, TEXT("Input Buffer Cleared"));
}

void UCPP_AC_InputBuffer::TryBufferAttack(EAttackType AttackType)
{
	if (!bCanBuffer) return;
	BufferedAttackType = AttackType;
	bIsInputBuffered = true;
	UE_LOG(LogTemp, Log, TEXT("Buffered %s"), *UEnum::GetValueAsString(AttackType))
}

EAttackType UCPP_AC_InputBuffer::GetBufferedAttackType() const
{
	return BufferedAttackType;
}


// Called every frame
void UCPP_AC_InputBuffer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}