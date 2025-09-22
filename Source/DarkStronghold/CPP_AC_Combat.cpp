#include "CPP_AC_Combat.h"
#include "CPP_DA_WeaponData.h"
#include "GameFramework/Character.h"
#include "Darkstronghold/CPP_AC_InputBuffer.h"
#include "Animation/AnimInstance.h"
#include "Darkstronghold/CPP_WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "CPP_I_Damageable.h"
#include "Darkstronghold/CPP_PlayerCharacter.h"

UCPP_AC_Combat::UCPP_AC_Combat()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCPP_AC_Combat::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		InputBufferComponent = CharacterOwner->FindComponentByClass<UCPP_AC_InputBuffer>();
		
		if (UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UCPP_AC_Combat::OnMontageEnded);
		}
	}
}

void UCPP_AC_Combat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsTracingHitbox && CharacterOwner && CurrentAttackData)
    {
        if (ACPP_PlayerCharacter* PlayerCharacter = Cast<ACPP_PlayerCharacter>(CharacterOwner))
        {
            if (ACPP_WeaponBase* CurrentWeapon = PlayerCharacter->GetCurrentWeapon())
            {
                if (UCPP_DA_WeaponData* WeaponData = CurrentWeapon->GetWeaponDataAsset())
                {
                    const TArray<FHitboxTraceData>& HitboxesToUse = CurrentAttackData->OverrideHitboxes.Num() > 0 ?
                                                                    CurrentAttackData->OverrideHitboxes :
                                                                    WeaponData->DefaultHitboxTraces;
                    
                    for (const FHitboxTraceData& Hitbox : HitboxesToUse)
                    {
                        if(UStaticMeshComponent* WeaponMesh = CurrentWeapon->FindComponentByClass<UStaticMeshComponent>())
                        {
                            const FVector Start = WeaponMesh->GetSocketLocation(Hitbox.StartSocket);
                            const FVector End = WeaponMesh->GetSocketLocation(Hitbox.EndSocket);

                            TArray<FHitResult> HitResults;
                            TArray<AActor*> ActorsToIgnore;
                            ActorsToIgnore.Add(CharacterOwner);
                            
                            UKismetSystemLibrary::SphereTraceMulti(GetWorld(), Start, End, Hitbox.Radius, 
                                UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore,
                                EDrawDebugTrace::ForDuration, HitResults, true, FLinearColor::Red, FLinearColor::Green, 2.0f);

                        	if (HitResults.Num() > 0)
                        	{
                        		for(const FHitResult& Hit : HitResults)
                        		{
                        			if(AActor* HitActor = Hit.GetActor())
                        			{
                        				if(!HitActorsInSwing.Contains(HitActor))
                        				{
                        					HitActorsInSwing.Add(HitActor);
            
                        					if(HitActor->Implements<UCPP_I_Damageable>())
                        					{
                        						const float DamageToApply = CurrentAttackData->Damage;
                        						ICPP_I_Damageable::Execute_ApplyDamage(HitActor, DamageToApply, Hit);
                        					}
                        				}
                        			}
                        		}
                        	}
                        }
                    }
                }
            }
        }
    }
}

void UCPP_AC_Combat::SetCharacterState(ECharacterState NewState)
{
	if (CurrentState == NewState) return;
	CurrentState = NewState;
}

void UCPP_AC_Combat::TryDodge()
{
	if (bIsDodgeOnCooldown) return;

	if (CurrentState == ECharacterState::Idle || CurrentState == ECharacterState::Attacking)
	{
		if (!DodgeMontage) return;

		SetCharacterState(ECharacterState::Dodging);
		
		if (UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DodgeMontage);
		}
		
		bIsDodgeOnCooldown = true;
		GetWorld()->GetTimerManager().SetTimer(DodgeCooldownTimerHandle, this, &UCPP_AC_Combat::ResetDodgeCooldown, DodgeCooldown, false);
	}
}

void UCPP_AC_Combat::HandleAttackInput(EAttackType AttackType)
{
	if (CurrentState == ECharacterState::Idle)
	{
		ACPP_PlayerCharacter* PlayerCharacter = Cast<ACPP_PlayerCharacter>(CharacterOwner);
		if (!PlayerCharacter) return;
		ACPP_WeaponBase* CurrentWeapon = PlayerCharacter->GetCurrentWeapon();
		if (!CurrentWeapon) return;
		UCPP_DA_WeaponData* WeaponData = CurrentWeapon->GetWeaponDataAsset();
		if (!WeaponData) return;

		PotentialCombos.Empty();
		
		for (const FComboData& Combo : WeaponData->Combos)
		{
			if (!Combo.AttackSequence.IsEmpty() && Combo.AttackSequence[0].AttackType == AttackType)
			{
				PotentialCombos.Add(&Combo);
			}
		}
		
		if (PotentialCombos.Num() > 0)
		{
			const FSingleAttackData& FirstAttack = PotentialCombos[0]->AttackSequence[0];
			
			if (UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance())
			{
				if(FirstAttack.AnimMontage)
				{
					ActiveComboName = PotentialCombos[0]->ComboName;
					UE_LOG(LogTemp, Log, TEXT("Executing %s sequence number %d"), *ActiveComboName.ToString(), ComboIndex + 1);
					AnimInstance->Montage_Play(FirstAttack.AnimMontage);
					CurrentAttackData = &FirstAttack;
					SetCharacterState(ECharacterState::Attacking);
					ComboIndex = 1;
					return;
				}
			}
		}
	}
	else if (CurrentState == ECharacterState::Attacking)
	{
		if (InputBufferComponent)
		{
			InputBufferComponent->TryBufferAttack(AttackType);
		}
	}
}

void UCPP_AC_Combat::StartWeaponTrace()
{
	bIsTracingHitbox = true;
	HitActorsInSwing.Empty();
}

void UCPP_AC_Combat::StopWeaponTrace()
{
	bIsTracingHitbox = false;
}

void UCPP_AC_Combat::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DodgeMontage)
	{
		if (CurrentState == ECharacterState::Dodging)
		{
			SetCharacterState(ECharacterState::Idle);
		}
		return; 
	}
	
	if (bInterrupted)
	{
		if (CurrentState == ECharacterState::Dodging)
		{
			return;
		}
		ResetCombo();
		return;
	}
	
	if (InputBufferComponent)
	{
		const EAttackType BufferedAttack = InputBufferComponent->GetBufferedAttackType();
		if (BufferedAttack != EAttackType::None)
		{
			InputBufferComponent->ClearBufferInput();
			ContinueCombo(BufferedAttack);
		}
		else
		{
			ResetCombo();
		}
	}
	else
	{
		ResetCombo();
	}
}

void UCPP_AC_Combat::ResetDodgeCooldown()
{
	bIsDodgeOnCooldown = false;
}

void UCPP_AC_Combat::ResetCombo()
{
	SetCharacterState(ECharacterState::Idle);
	ComboIndex = 0;
	PotentialCombos.Empty();
	CurrentAttackData = nullptr;
	ActiveComboName = FName();

	if (InputBufferComponent)
	{
		InputBufferComponent->ClearBufferInput();
	}
}

void UCPP_AC_Combat::ContinueCombo(EAttackType BufferedAttackType)
{
	if (PotentialCombos.IsEmpty())
	{
		ResetCombo();
		return;
	}

	TArray<const FComboData*> NextPotentialCombos;

	for (const FComboData* CurrentPotentialCombo : PotentialCombos)
	{
		if (CurrentPotentialCombo->AttackSequence.IsValidIndex(ComboIndex))
		{
			if (CurrentPotentialCombo->AttackSequence[ComboIndex].AttackType == BufferedAttackType)
			{
				NextPotentialCombos.Add(CurrentPotentialCombo);
			}
		}
	}

	PotentialCombos = NextPotentialCombos;

	if (PotentialCombos.Num() > 0)
	{
		const FSingleAttackData& NextAttack = PotentialCombos[0]->AttackSequence[ComboIndex];
		
		if (UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance())
		{
			if(NextAttack.AnimMontage)
			{
				ActiveComboName = PotentialCombos[0]->ComboName;
				UE_LOG(LogTemp, Log, TEXT("Executing %s sequence number %d"), *ActiveComboName.ToString(), ComboIndex + 1);
				AnimInstance->Montage_Play(NextAttack.AnimMontage);
				CurrentAttackData = &NextAttack;
				ComboIndex++;
				return;
			}
		}
	}
	
	ResetCombo();
}