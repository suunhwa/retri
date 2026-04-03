// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Player/Data/PlayerStatData.h"
#include "ReTri/ReTri.h"
#include "ReTriGameInstance.generated.h"

class UStatComponent;
class UHealthComponent;
/**
 * 
 */
UCLASS()
class RETRI_API UReTriGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	// =========== Player ===============================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	FPlayerStatInfo CurPlayerStats;
	
	// 플레이어 컴포넌트 레퍼런스
	UPROPERTY()
	TObjectPtr<UStatComponent> StatComp;

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComp;
	
	// 스킬 데이터 (DT_PlayerSkills)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Data")
	UDataTable* SkillDataTable;

	// 레벨/경험치 데이터 (DT_LevelExp)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Data")
	UDataTable* LevelDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	bool bHasSavedStats = false;
	
	// 스킬 슬롯 매핑 저장 (드래그 앤 드롭으로 위치 바뀌어도 유지)
	// Key: EAbilitySlot, Value: 해당 슬롯에 장착된 스킬 클래스
	UPROPERTY()
	TMap<uint8, UClass*> SavedSkillSlots;
	
	// todo Player Stat 부분 정리 필
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	TSubclassOf<class UReTriGameData> GameDataClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player|Stat")
	UReTriGameData* GameData;
	
	// =========== Level ===============================================
	//! 상호작용 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Interaction")
	UDataTable* InteractionData;

	//! 절차적 맵 생성 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|NodeMap")
	UDataTable* MapUIData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|NodeMap")
	TSubclassOf<class ALootGoldCoinPot> GoldCoinPotClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|NodeMap")
	TSubclassOf<class ALootDreamPowderPillar> DreamPowderClass;
	// =========== Level ===============================================
	
	void DebugStat() const
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
};
