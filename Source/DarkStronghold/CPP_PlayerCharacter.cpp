// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CPP_AC_InputBuffer.h"
#include "CPP_AC_Combat.h"

// Sets default values
ACPP_PlayerCharacter::ACPP_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InputBufferComponent = CreateDefaultSubobject<UCPP_AC_InputBuffer>(TEXT("Input Buffer Component"));
}

// Called when the game starts or when spawned
void ACPP_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ACPP_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::Interact);
		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::LightAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::HeavyAttack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::Dodge);

	}
}

void ACPP_PlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// Ottieni la rotazione del controller (la direzione in cui la telecamera sta guardando)
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y); // W/S
		AddMovementInput(RightDirection, MovementVector.X); // A/D
	}
}

void ACPP_PlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACPP_PlayerCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact action triggered"));
	// TODO: Implementa qui la logica di interazione
}

void ACPP_PlayerCharacter::LightAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Light Attack action triggered"));
	InputBufferComponent->TryBufferAttack(EAttackType::LightAttack);
}

void ACPP_PlayerCharacter::HeavyAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Heavy Attack action triggered"));
	InputBufferComponent->TryBufferAttack(EAttackType::HeavyAttack);
}

void ACPP_PlayerCharacter::Dodge()
{
	UE_LOG(LogTemp, Warning, TEXT("Dodge action triggered"));
	// TODO: Implementa qui la logica della schivata
}

// Called every frame
void ACPP_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
