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

/** Active: 입력으로 발동 / Passive: 자동 발동 */
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Active  UMETA(DisplayName = "Active"),
	Passive UMETA(DisplayName = "Passive"),
};

/** 스킬 분류 (원작 기억 카테고리 기준) */
UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
	Innate   UMETA(DisplayName = "Innate"),    // 기본 공격 등 기본 내장
	Identity UMETA(DisplayName = "Identity"),  // 정체성 기억 (패시브)
	Traveler UMETA(DisplayName = "Traveler"),  // 여행자 기억 (고유 스킬)
	Acquired UMETA(DisplayName = "Acquired"),  // 공용 기억 (획득 스킬)
};

/** 쿨다운 방식 */
UENUM(BlueprintType)
enum class ESkillCooldownType : uint8
{
	Cooldown UMETA(DisplayName = "Cooldown"), // 일반 쿨다운
	Charge   UMETA(DisplayName = "Charge"),   // 충전식
	None     UMETA(DisplayName = "None"),     // 패시브 등 쿨다운 없음
};

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
	ESkillType SkillType = ESkillType::Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillCategory SkillCategory = ESkillCategory::Innate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillCooldownType CooldownType = ESkillCooldownType::Cooldown;

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

	// 현재 미사용 (0으로 고정). 상점에서 스킬 구매 시 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeCostGold = 0;

	// 강화 초기 비용. 실제 비용 = UpgradeCostDreamDust + (강화 횟수 * 10)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeCostDreamDust = 0;

	// 0 = 무제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxUpgradeLevel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
};
