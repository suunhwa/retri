/**
 * FPlayerLevelData
 *
 * DataTable row struct for player level definitions.
 * Import DT_LevelExp.csv with this row type.
 *
 * RequiredExp: 해당 레벨 도달에 필요한 누적 경험치
 * 스탯 수치는 나무위키 기준 라세르타 기본값 + 레벨당 성장치 적용
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerLevelData.generated.h"

USTRUCT(BlueprintType)
struct RETRI_API FPlayerLevelData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	// 이 레벨 도달에 필요한 누적 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredExp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 220.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower = 43.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpellPower = 41.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed = 1.41f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 520.f;
};
