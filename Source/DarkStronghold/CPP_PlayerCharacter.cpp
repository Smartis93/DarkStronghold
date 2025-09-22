// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CPP_AC_InputBuffer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPP_I_Pickup.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "CPP_WeaponBase.h"
#include "CPP_AC_Combat.h"

// Sets default values
ACPP_PlayerCharacter::ACPP_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// --- Configurazione Movimento ---
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// --- Creazione Componenti ---
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	InputBufferComponent = CreateDefaultSubobject<UCPP_AC_InputBuffer>(TEXT("Input Buffer Component"));
	CombatComponent = CreateDefaultSubobject<UCPP_AC_Combat>(TEXT("Combat Component"));
	InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionTrigger"));

	// --- Setup Gerarchia e Proprietà ---
	SpringArmComponent->SetupAttachment(RootComponent);
	ThirdPersonCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	InteractionTrigger->SetupAttachment(RootComponent);
	
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	ThirdPersonCameraComponent->bUsePawnControlRotation = false;
	InteractionTrigger->SetSphereRadius(150.0f);

	// --- Binding Eventi ---
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACPP_PlayerCharacter::OnInteractionOverlapBegin);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &ACPP_PlayerCharacter::OnInteractionOverlapEnd);
}

void ACPP_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void ACPP_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::DropWeapon);
	}
}

// --- Gestione Input ---

void ACPP_PlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
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

void ACPP_PlayerCharacter::LightAttack()
{
	if (CombatComponent)
	{
		CombatComponent->HandleAttackInput(EAttackType::LightAttack);
	}
}

void ACPP_PlayerCharacter::HeavyAttack()
{
	if (CombatComponent)
	{
		CombatComponent->HandleAttackInput(EAttackType::HeavyAttack);
	}
}

void ACPP_PlayerCharacter::Dodge()
{
	if (CombatComponent)
	{
		CombatComponent->TryDodge();
	}
}

void ACPP_PlayerCharacter::DropWeapon()
{
	if (CurrentWeapon && CombatComponent)
	{
		ICPP_I_Pickup::Execute_Drop(CurrentWeapon);
		CurrentWeapon = nullptr;
	}
}

// --- Logica di Interazione ---

void ACPP_PlayerCharacter::Interact()
{
	if (OverlappedInteractable)
	{
		AActor* ActorToPickup = OverlappedInteractable;
		
		if (CurrentWeapon)
		{
			ICPP_I_Pickup::Execute_Drop(CurrentWeapon);
		}
		
		ICPP_I_Pickup::Execute_Pickup(ActorToPickup, this);
		
		if (ACPP_WeaponBase* PickedUpWeapon = Cast<ACPP_WeaponBase>(ActorToPickup))
		{
			SetEquippedWeapon(PickedUpWeapon);
		}
	}
}

void ACPP_PlayerCharacter::OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UCPP_I_Pickup>())
	{
		OverlappedInteractable = OtherActor;
	}
}

void ACPP_PlayerCharacter::OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor == OverlappedInteractable)
	{
		OverlappedInteractable = nullptr;
	}
}

void ACPP_PlayerCharacter::SetEquippedWeapon(ACPP_WeaponBase* NewWeapon)
{
	CurrentWeapon = NewWeapon;
}

ACPP_WeaponBase* ACPP_PlayerCharacter::GetCurrentWeapon() const
{
	return CurrentWeapon;
}