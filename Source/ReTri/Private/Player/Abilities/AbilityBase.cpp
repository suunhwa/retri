// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/AbilityBase.h"

#include "GameFramework/Character.h"

bool UAbilityBase::TryActivate(ACharacter* Owner)
{
	if (!Owner || bIsOnCooldown || !CanActivate(Owner)) return false;
	
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
