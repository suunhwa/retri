// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"
#include "ReTriGameInstance.h"
#include "Elements/Columns/TypedElementRevisionControlColumns.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::SyncToGameInstance()
{
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return;
	GI->CurPlayerStats = GetStatInfo();
	GI->bHasSavedStats = true;

	UE_LOG(LogTemp,
	       Warning,
	       TEXT("[StatSync] Gold: %d | DreamDust: %d"),
	       GI->CurPlayerStats.Gold,
	       GI->CurPlayerStats.DreamDust);
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());

	if (GI && GI->bHasSavedStats)
	{
		const FPlayerStatInfo SavedInfo = GI->CurPlayerStats;

		Gold = SavedInfo.Gold;
		DreamDust = SavedInfo.DreamDust;
		CurrentLevel = SavedInfo.CurrentLevel;
		CurrentExp = SavedInfo.CurrentExp;
		// Base 스탯을 현재 레벨로 복원
		LoadStatsForLevel(CurrentLevel);

		AddedMaxHP = SavedInfo.AddedMaxHP;
		AddedAttackPower = SavedInfo.AddedAttackPower;
		AddedSpellPower = SavedInfo.AddedSpellPower;
		AddedAttackSpeed = SavedInfo.AddedAttackSpeed;
		AddedCritRate = SavedInfo.AddedCritRate;
		AddedCritMultiplier = SavedInfo.AddedCritMultiplier;
		AddedMoveSpeed = SavedInfo.AddedMoveSpeed;
		AddedDefense = SavedInfo.AddedDefense;
		AddedMemoryAcceleration = SavedInfo.AddedMemoryAcceleration;
		AddedAttackRange = SavedInfo.AddedAttackRange;
		AddedProjectileSpeed = SavedInfo.AddedProjectileSpeed;
		AddedDashCount = SavedInfo.AddedDashCount;
		AddedDashCooldown = SavedInfo.AddedDashCooldown;
		AddedBurnDamageBonus = SavedInfo.AddedBurnDamageBonus;
	}

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
		AddedAttackSpeed *= Delta;
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
	FPlayerLevelGrowthData* Row = GI->LevelDataTable->FindRow<FPlayerLevelGrowthData>(
		RowName,
		TEXT("LoadStatsForLevel"));
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

void UStatComponent::AddExp(int32 Amount)
{
	if (Amount <= 0) return;
	CurrentExp += Amount;

	// 레벨업 체크 — 누적 경험치가 다음 레벨 조건 충족 시 반복 처리
	int32 NextLevel = CurrentLevel + 1;
	int32 Required = GetRequiredExpForLevel(NextLevel);
	while (Required != -1 && CurrentExp >= Required && NextLevel <= MaxLevel)
	{
		LoadStatsForLevel(NextLevel);
		OnLevelUp.Broadcast(NextLevel);
		NextLevel++;
		Required = GetRequiredExpForLevel(NextLevel);
	}

	// ExpBar 브로드캐스트: 다음 레벨 필요 경험치 (-1이면 맥스 레벨)
	const int32 RequiredForNext = GetRequiredExpForLevel(CurrentLevel + 1);

	UE_LOG(LogTemp, Warning, TEXT("[EXP] Lv.%d | EXP: %d / %d"), CurrentLevel, CurrentExp, RequiredForNext);

	OnExpChanged.Broadcast(CurrentExp, RequiredForNext, CurrentLevel);

	SyncToGameInstance();
}

int32 UStatComponent::GetRequiredExpForLevel(int32 Level) const
{
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI || !GI->LevelDataTable) return -1;

	const FName RowName = FName(*FString::Printf(TEXT("Level_%d"), Level));
	FPlayerLevelGrowthData* Row = GI->LevelDataTable->FindRow<FPlayerLevelGrowthData>(
		RowName,
		TEXT("GetRequiredExpForLevel"));
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
	Info.CurrentLevel = CurrentLevel;
	Info.CurrentExp = CurrentExp;

	// Added 스탯 저장
	Info.AddedMaxHP = AddedMaxHP;
	Info.AddedAttackPower = AddedAttackPower;
	Info.AddedSpellPower = AddedSpellPower;
	Info.AddedAttackSpeed = AddedAttackSpeed;
	Info.AddedCritRate = AddedCritRate;
	Info.AddedCritMultiplier = AddedCritMultiplier;
	Info.AddedMoveSpeed = AddedMoveSpeed;
	Info.AddedDefense = AddedDefense;
	Info.AddedMemoryAcceleration = AddedMemoryAcceleration;
	Info.AddedAttackRange = AddedAttackRange;
	Info.AddedProjectileSpeed = AddedProjectileSpeed;
	Info.AddedDashCount = AddedDashCount;
	Info.AddedDashCooldown = AddedDashCooldown;
	Info.AddedBurnDamageBonus = AddedBurnDamageBonus;
	return Info;
}
