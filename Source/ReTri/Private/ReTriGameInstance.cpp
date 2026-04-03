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
	if (nullptr == GameData)  
		UE_LOG(LogTemp, Error, TEXT("[UReTriGameInstance] GameDataClass 할당안됨"));

	UMapSubSystem* MapSub = GetSubsystem<UMapSubSystem>();
	if (MapSub)
	{
		MapSub->InteractionData = this->InteractionData;
		MapSub->MapUIData = this->MapUIData;
		MapSub->SkillDataTable = this->SkillDataTable;
		
		MapSub->GoldCoinPotClass = this->GoldCoinPotClass;
		MapSub->DreamPowderClass = this->DreamPowderClass;
		
		MapSub->ProceduralGenerateMap();
	}
}

void UReTriGameInstance::DebugStat() const
{
	UE_LOG(jiwon, Log, TEXT("==================== Player Stats ===================="));
	// 기본 스탯
	UE_LOG(jiwon, Log, TEXT("[Base Stats] HP: %.2f / AD: %.2f"), HealthComp->GetCurrentHP(), StatComp->GetAttackPower());
	UE_LOG(jiwon, Log, TEXT("[Base Stats] Gold: %d / DreamPowder: %d / Exp: %d"), StatComp->GetGold(), StatComp->GetDreamDust(), StatComp->GetCurrentExp());
	// 보조 스탯
	UE_LOG(jiwon, Log, TEXT("[Secondary Stats] AS: %.2f / CoolTime: %.2f"), StatComp->GetAttackSpeed(), StatComp->GetMemoryAcceleration());
	// 유틸리티 및 원소 스탯
	UE_LOG(jiwon, Log, TEXT("[Utility] MoveSpeed: %.2f"), StatComp->GetMoveSpeed());
	UE_LOG(jiwon, Log, TEXT("[Elemental] FireDamage: %.2f"), StatComp->GetBurnDamageBonus());
	UE_LOG(jiwon, Log, TEXT("======================================================"));
}
