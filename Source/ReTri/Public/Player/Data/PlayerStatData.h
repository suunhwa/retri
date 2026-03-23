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
	float MoveSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float AttackDamage = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float AttackSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float DashCooldown = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float MaxHP = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float FireDamage = 0.f;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentLevel = 1;
};

// -------------------------------------------------------
// DataTable 행 Struct — DT_PlayerStat 에셋에서 사용
// CSV Row Name: Level_1, Level_2, ...
// -------------------------------------------------------
USTRUCT(BlueprintType)
struct RETRI_API FPlayerStatRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float MoveSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float AttackDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float AttackSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float DashCooldown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float MaxHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat")
	float FireDamage = 0.f;

	// 이 레벨에 도달하기 위해 필요한 누적 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	int32 RequiredExp = 0;
};
