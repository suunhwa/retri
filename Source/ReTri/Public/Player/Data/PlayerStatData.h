// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerStatData.generated.h"

// -------------------------------------------------------
// 성소 등 외부 시스템에서 플레이어 스탯을 읽을 때 쓰는 Struct
// StatComponent::GetStatInfo()로 받아감
// -------------------------------------------------------
USTRUCT(BlueprintType)
struct RETRI_API FPlayerStatInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float MaxHP = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	float AttackDamage = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float AbilityPower = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	float AttackSpeed = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	float CritChance = 0.f; //치명타 확률 (0 ~ 100)

	UPROPERTY(BlueprintReadOnly)
	float MoveSpeed = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	float Defense = 0.f; // 방어력
	
	UPROPERTY(BlueprintReadOnly)
	float MemoryHaste = 0.f; // 기억 가속
	
	UPROPERTY(BlueprintReadOnly)
	float DashCooldown = 5.f;
	
	UPROPERTY(BlueprintReadOnly)
	float FireDamage = 0.f;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentLevel = 1;
};

// -------------------------------------------------------
// DataTable 행 Struct — DT_PlayerStat 에셋에서 사용
// 플레이어 기본 스탯 & 레벨당 growth rate 저장
// -------------------------------------------------------
USTRUCT(BlueprintType)
struct RETRI_API FPlayerStatRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseHP = 220.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseAD = 43.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseAP = 41.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseAS = 1.41f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseCrit = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseMS = 520.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseDefense = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseHaste = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseFire = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base")
	float BaseDashCooldown = 5.f;
	
	// 레벨당 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float HPGrowth = 44.f; // 220 * 0.2 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float ADGrowth = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float APGrowth = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float ASGrowth = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float CritGrowth = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float MSGrowth = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float DefenseGrowth = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float HasteGrowth = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float FireGrowth = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Growth")
	float DashCooldownGrowth = 0.f; // 레벨당 감소량
};

// -------------------------------------------------------
// DataTable 행 Struct — DT_ExpTable 에셋에서 사용
// CSV Row Name: Level_1, Level_2, ...
// -------------------------------------------------------
USTRUCT(BlueprintType)
struct RETRI_API FExpTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	// 레벨에 도달하기 위해 필요한 누적 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	int32 RequiredExp = 0;
};