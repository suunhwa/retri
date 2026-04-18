

#include "ReTriGameInstance.h"

#include "ReTriGameData.h"
#include "Player/Components/HealthComponent.h"
#include "Player/Components/StatComponent.h"

void UReTriGameInstance::Init()
{
	Super::Init();
	
	if (GameDataClass)
	{
		GameData = NewObject<UReTriGameData>(this, GameDataClass);
	}
}


void UReTriGameInstance::SaveStatSnapshot()
{
	if (HealthComp)
	{
		Snapshot_MaxHP = HealthComp->GetMaxHP();
	}
	
	if (StatComp)
	{
		Snapshot_AttackPower        = StatComp->GetAttackPower();
		Snapshot_SpellPower         = StatComp->GetSpellPower();
		Snapshot_AttackSpeed        = StatComp->GetAttackSpeed();
		Snapshot_MemoryAcceleration = StatComp->GetMemoryAcceleration();
		Snapshot_CritRate           = StatComp->GetCritRate();
		Snapshot_BurnDamageBonus    = StatComp->GetBurnDamageBonus();
		Snapshot_CurrentExp         = StatComp->GetCurrentExp();
	}
}

void UReTriGameInstance::DebugStat() const
{
	// UE_LOG(LogTemp, Log, TEXT("==================== Player Stats ===================="));
	// UE_LOG(LogTemp, Log, TEXT("[Base Stats] HP: %.2f / AD: %.2f"), HealthComp->GetCurrentHP(), StatComp->GetAttackPower());
	// UE_LOG(LogTemp, Log, TEXT("[Base Stats] Gold: %d / DreamPowder: %d / Exp: %d"), StatComp->GetGold(), StatComp->GetDreamDust(), StatComp->GetCurrentExp());
	// UE_LOG(LogTemp, Log, TEXT("[Secondary Stats] AS: %.2f / CoolTime: %.2f"), StatComp->GetAttackSpeed(), StatComp->GetMemoryAcceleration());
	// UE_LOG(LogTemp, Log, TEXT("[Utility] MoveSpeed: %.2f"), StatComp->GetMoveSpeed());
	// UE_LOG(LogTemp, Log, TEXT("[Elemental] FireDamage: %.2f"), StatComp->GetBurnDamageBonus());
	// UE_LOG(LogTemp, Log, TEXT("======================================================"));
}
