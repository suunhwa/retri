// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"
#include "ReTriGameInstance.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::SyncToGameInstance()
{
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return;
	GI->CurPlayerStats = GetStatInfo();
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
	if (HealthComp)
	{
		HealthComp->SetMaxHP(GetMaxHP(), true);
	}

	SyncToGameInstance();
}

void UStatComponent::BeginBatch()
{
	bBatchMode = true;
}

void UStatComponent::EndBatch()
{
	bBatchMode = false;
	SyncToGameInstance();
}

void UStatComponent::ApplyStatModifier(EStatTypes Type, float Delta)
{
	float NewValue = 0.f;

	switch (Type)
	{
	case EStatTypes::Gold:
		Gold = FMath::Max(0, Gold + static_cast<int32>(Delta));
		NewValue = static_cast<float>(Gold);
		break;

	case EStatTypes::DreamDust:
		DreamDust = FMath::Max(0, DreamDust + static_cast<int32>(Delta));
		NewValue = static_cast<float>(DreamDust);
		break;

	case EStatTypes::MaxHP:
		AddedMaxHP += Delta;
		NewValue = GetMaxHP();
		if (HealthComp)
			HealthComp->SetMaxHP(GetMaxHP(), false);
		break;

	case EStatTypes::AttackPower:
		AddedAttackPower += Delta;
		NewValue = GetAttackPower();
		break;

	case EStatTypes::SpellPower:
		AddedSpellPower += Delta;
		NewValue = GetSpellPower();
		break;

	case EStatTypes::AttackSpeed:
		AddedAttackSpeed += Delta;
		NewValue = GetAttackSpeed();
		break;

	case EStatTypes::CritRate:
		AddedCritRate += Delta;
		NewValue = GetCritRate();
		break;

	case EStatTypes::CritMultiplier:
		AddedCritMultiplier += Delta;
		NewValue = GetCritMultiplier();
		break;

	case EStatTypes::MoveSpeed:
		AddedMoveSpeed += Delta;
		NewValue = GetMoveSpeed();
		break;

	case EStatTypes::Defense:
		AddedDefense += Delta;
		NewValue = GetDefense();
		break;

	case EStatTypes::MemoryAcceleration:
		AddedMemoryAcceleration += Delta;
		NewValue = GetMemoryAcceleration();
		break;

	case EStatTypes::AttackRange:
		AddedAttackRange += Delta;
		NewValue = GetAttackRange();
		break;

	case EStatTypes::ProjectileSpeed:
		AddedProjectileSpeed += Delta;
		NewValue = GetProjectileSpeed();
		break;

	case EStatTypes::DashCount:
		AddedDashCount += static_cast<int32>(Delta);
		NewValue = static_cast<float>(GetDashCount());
		break;

	case EStatTypes::DashCooldown:
		AddedDashCooldown += Delta;
		NewValue = GetDashCooldown();
		break;

	case EStatTypes::BurnDamageBonus:
		AddedBurnDamageBonus += Delta;
		NewValue = GetBurnDamageBonus();
		break;
	}

	OnStatChanged.Broadcast(Type, NewValue);
	if (!bBatchMode) SyncToGameInstance();
}

bool UStatComponent::SpendGold(int32 Cost)
{
	if (Gold < Cost) return false;
	Gold -= Cost;
	OnStatChanged.Broadcast(EStatTypes::Gold, static_cast<float>(Gold));
	SyncToGameInstance();
	return true;
}

bool UStatComponent::SpendDreamDust(int32 Cost)
{
	if (DreamDust < Cost) return false;
	DreamDust -= Cost;
	OnStatChanged.Broadcast(EStatTypes::DreamDust, static_cast<float>(DreamDust));
	SyncToGameInstance();
	return true;
}

void UStatComponent::LoadStatsForLevel(int32 Level)
{
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI || !GI->LevelDataTable) return;

	const FName RowName = FName(*FString::Printf(TEXT("Level_%d"), Level));
	FPlayerLevelGrowthData* Row = GI->LevelDataTable->FindRow<FPlayerLevelGrowthData>(RowName, TEXT("LoadStatsForLevel"));
	if (!Row) return;

	// Base만 덮어씀 — Added(성소) 보존
	CurrentLevel = Level;
	BaseMaxHP = Row->MaxHP;
	BaseAttackPower = Row->AttackPower;
	BaseSpellPower = Row->SpellPower;
	BaseAttackSpeed = Row->AttackSpeed;
	BaseMoveSpeed = Row->MoveSpeed;

	if (HealthComp)
		HealthComp->SetMaxHP(GetMaxHP(), false);

	BroadcastLevelStatsChanged();
	// OnStatChanged.Broadcast(EStatTypes::MaxHP, GetMaxHP());
	SyncToGameInstance();
}

void UStatComponent::BroadcastLevelStatsChanged()
{
	OnStatChanged.Broadcast(EStatTypes::MaxHP, GetMaxHP());
	OnStatChanged.Broadcast(EStatTypes::AttackPower, GetAttackPower());
	OnStatChanged.Broadcast(EStatTypes::SpellPower, GetSpellPower());
	OnStatChanged.Broadcast(EStatTypes::AttackSpeed, GetAttackSpeed());
	OnStatChanged.Broadcast(EStatTypes::MoveSpeed, GetMoveSpeed());
}

int32 UStatComponent::GetRequiredExpForLevel(int32 Level) const
{
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI || !GI->LevelDataTable) return -1;

	const FName RowName = FName(*FString::Printf(TEXT("Level_%d"), Level));
	FPlayerLevelGrowthData* Row = GI->LevelDataTable->FindRow<FPlayerLevelGrowthData>(RowName, TEXT("GetRequiredExpForLevel"));
	return Row ? Row->RequiredExp : -1;
}

FPlayerStatInfo UStatComponent::GetStatInfo() const
{
	FPlayerStatInfo Info;
	Info.Gold = Gold;
	Info.DreamDust = DreamDust;
	Info.MaxHP = GetMaxHP();
	Info.AttackPower = GetAttackPower();
	Info.SpellPower = GetSpellPower();
	Info.AttackSpeed = GetAttackSpeed();
	Info.CritRate = GetCritRate();
	Info.CritMultiplier = GetCritMultiplier();
	Info.MoveSpeed = GetMoveSpeed();
	Info.Defense = GetDefense();
	Info.MemoryAcceleration = GetMemoryAcceleration();
	Info.AttackRange = GetAttackRange();
	Info.ProjectileSpeed = GetProjectileSpeed();
	Info.DashCount = GetDashCount();
	Info.DashCooldown = GetDashCooldown();
	Info.BurnDamageBonus = GetBurnDamageBonus();
	return Info;
}
