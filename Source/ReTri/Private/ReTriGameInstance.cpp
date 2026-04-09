// Fill out your copyright notice in the Description page of Project Settings.


#include "ReTriGameInstance.h"

#include "MapSubSystem.h"
#include "ReTriGameData.h"
#include "Player/Components/HealthComponent.h"
#include "Player/Components/StatComponent.h"

void UReTriGameInstance::Init()
{
	Super::Init();
	
	if (GameDataClass)
		GameData = NewObject<UReTriGameData>(this, GameDataClass);
	// if (nullptr == GameData)  
	// 	UE_LOG(LogTemp, Error, TEXT("[UReTriGameInstance] GameDataClass 할당안됨"));
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
	// UE_LOG(jiwon, Log, TEXT("==================== Player Stats ===================="));
	// // 기본 스탯
	// UE_LOG(jiwon, Log, TEXT("[Base Stats] HP: %.2f / AD: %.2f"), HealthComp->GetCurrentHP(), StatComp->GetAttackPower());
	// UE_LOG(jiwon, Log, TEXT("[Base Stats] Gold: %d / DreamPowder: %d / Exp: %d"), StatComp->GetGold(), StatComp->GetDreamDust(), StatComp->GetCurrentExp());
	// // 보조 스탯
	// UE_LOG(jiwon, Log, TEXT("[Secondary Stats] AS: %.2f / CoolTime: %.2f"), StatComp->GetAttackSpeed(), StatComp->GetMemoryAcceleration());
	// // 유틸리티 및 원소 스탯
	// UE_LOG(jiwon, Log, TEXT("[Utility] MoveSpeed: %.2f"), StatComp->GetMoveSpeed());
	// UE_LOG(jiwon, Log, TEXT("[Elemental] FireDamage: %.2f"), StatComp->GetBurnDamageBonus());
	// UE_LOG(jiwon, Log, TEXT("======================================================"));
}
