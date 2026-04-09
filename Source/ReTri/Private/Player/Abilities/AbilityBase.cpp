// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/AbilityBase.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

bool UAbilityBase::TryActivate(ACharacter* Owner)
{
	if (!Owner || bIsOnCooldown || !CanActivate(Owner)) return false;

	// 마우스 방향으로 회전 (ShouldRotateToMouse() == false인 스킬은 건너뜀)
	if (ShouldRotateToMouse())
	{
		if (APlayerController* PC = Cast<APlayerController>(Owner->GetController()))
		{
			FVector WorldPos;
			FVector WorldDir;
			if (PC->DeprojectMousePositionToWorld(WorldPos, WorldDir))
			{
				float T = (Owner->GetActorLocation().Z - WorldPos.Z) / WorldDir.Z;
				FVector MouseWorld = WorldPos + WorldDir * T;

				FVector Dir = MouseWorld - Owner->GetActorLocation();
				Dir.Z = 0.f;
				if (!Dir.IsNearlyZero())
				{
					Dir.Normalize();
					Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
					Owner->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));

					FTimerHandle RotHandle;
					TWeakObjectPtr<ACharacter> WeakOwner = Owner;
					Owner->GetWorldTimerManager().SetTimer(RotHandle, [WeakOwner]()
					{
						if (WeakOwner.IsValid()) WeakOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
					}, 0.8f, false);
					
					// FTimerHandle RotHandle;
					// Owner->GetWorldTimerManager().SetTimer(RotHandle, [Owner]()
					// {
					// 	if (Owner) Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
					// }, 0.8f, false);
				}
			}
		}
	}

	if (CastMontage)
	{
		Owner->PlayAnimMontage(CastMontage);
	}

	Activate(Owner);
	OnAbilityActivated.Broadcast();
	StartCooldown(Owner->GetWorld());
	return true;
}

void UAbilityBase::InitFromDataTable(UDataTable* DataTable, FName RowName)
{
	if (!DataTable) return;

	const FPlayerSkillData* Row = DataTable->FindRow<FPlayerSkillData>(RowName, TEXT("InitFromDataTable"));
	if (!Row) return;

	Cooldown   = Row->BaseCooldown;
	BaseDamage = Row->BaseDamage;
	MaxCharges = Row->MaxCharges;
	Duration   = Row->Duration;
	if (Row->Icon)
		Icon = Row->Icon;
	if (Row->CastMontage)
		CastMontage = Row->CastMontage;
}

void UAbilityBase::StartCooldown(UWorld* World)
{
	if (!World || Cooldown <= 0.f) return;
	
	bIsOnCooldown = true;
	RemainingCooldown = Cooldown;
	OnCooldownChanged.Broadcast(RemainingCooldown, Cooldown);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UAbilityBase::TickCooldown);
	World->GetTimerManager().SetTimer(CooldownTimer, TimerDelegate, 0.05f, true);
}

void UAbilityBase::TickCooldown()
{
	RemainingCooldown -= 0.05f;
	
	if (RemainingCooldown <= 0.f)
	{
		RemainingCooldown = 0.f;
		bIsOnCooldown = false;
		
		UWorld* World = GetWorld();
		
		if (World)
		{
			World->GetTimerManager().ClearTimer(CooldownTimer);
		}
	}
	
	OnCooldownChanged.Broadcast(RemainingCooldown, Cooldown);
}
