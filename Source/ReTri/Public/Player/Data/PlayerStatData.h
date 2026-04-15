/**
 * FPlayerStatInfo
 *
 * 플레이어 스탯 스냅샷 DTO.
 * StatComponent가 진짜 값을 들고 있고, 이 struct는 다른 시스템에 값을 넘겨줄 때만 사용.
 * 로직/메서드 없음 — 순수 데이터 필드만.
 */

#pragma once

#include "CoreMinimal.h"
#include "PlayerStatData.generated.h"

USTRUCT(BlueprintType)
struct RETRI_API FPlayerStatInfo
{
	GENERATED_BODY()

	// --- Currency --------------------------------------------------------------
	// 상점 / 성소 / 분해의 성소 등 주 화폐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Currency")
	int32 Gold = 0;

	// 빛나는 우물에서 기억 강화에 소비
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Currency")
	int32 DreamDust = 0;

	// --- Health --------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Health")
	float MaxHP = 220.f;

	// --- Combat --------------------------------------------------------------
	// 기본 공격 피해량 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Combat")
	float AttackPower = 43.f;

	// 대부분의 기억/스킬 피해량 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Combat")
	float SpellPower = 41.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Combat")
	float Defense = 0.f;

	// 0.0 ~ 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Combat")
	float CritRate = 0.05f;

	// 치명타 배율 (2.0 = 200%)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Combat")
	float CritMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Combat")
	float AttackRange = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Combat")
	float ProjectileSpeed = 1200.f;

	// 초당 기본 공격 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Combat")
	float AttackSpeed = 1.41f;

	// --- Utility --------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Utility")
	float MoveSpeed = 520.f;

	// 기억 가속 — 쿨다운 배율: 100 / (100 + MemoryAcceleration)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Utility")
	float MemoryAcceleration = 0.f;

	// --- Elemental --------------------------------------------------------------
	// 화상 피해 보너스 배율 (0.0 = 보너스 없음)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Elemental")
	float BurnDamageBonus = 0.f;

	// --- Dash --------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Dash")
	int32 DashCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Dash")
	float DashCooldown = 5.f;

	// --- Level / Exp --------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Level")
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Level")
	int32 CurrentExp = 0;

	// --- Added: 성소/강화에서 누적, 레벨업 후에도 유지 ----------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedMaxHP = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedAttackPower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedSpellPower = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedAttackSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedCritRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedCritMultiplier = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedMoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedDefense = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedMemoryAcceleration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedAttackRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedProjectileSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	int32 AddedDashCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedDashCooldown = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats|Added")
	float AddedBurnDamageBonus = 0.f;
};
