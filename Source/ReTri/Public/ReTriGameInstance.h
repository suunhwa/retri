// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Player/Data/PlayerStatData.h"
#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"
#include "ReTriGameInstance.generated.h"

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
};
