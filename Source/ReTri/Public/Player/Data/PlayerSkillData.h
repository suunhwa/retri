/**
 * FPlayerSkillData
 *
 * DataTable row struct for player skill definitions.
 * Import DT_PlayerSkills.csv with this row type.
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerSkillData.generated.h"

USTRUCT(BlueprintType)
struct RETRI_API FPlayerSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillNameKR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillCategory;

	// "Cooldown": standard cooldown skill / "Charge": charge-based skill (charged by damage dealt) / "None": passive
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CooldownType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseCooldown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AoeRadius = 0.f;

	// DoT or CC 지속시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.f;

	// 충전식 스킬 최대 충전 횟수 (CooldownType == "Charge"일 때만 유효)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCharges = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockbackForce = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedMultiplier = 1.f;

	// 현재 미사용 (0으로 고정). 추후 골드 강화 비용 도입 시 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeCostGold = 0;

	// 강화 초기 비용. 실제 비용 = UpgradeCostDreamDust + (강화 횟수 * 10)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeCostDreamDust = 0;

	// 0 = 무제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxUpgradeLevel = 0;
};
