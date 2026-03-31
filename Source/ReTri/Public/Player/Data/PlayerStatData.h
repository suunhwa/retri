/**
 * FPlayerStatInfo
 *
 * Holds all runtime stats for a player character.
 * Used as the primary data container for combat, movement, and currency.
 *
 * Player base stats are set as default values.
 * Per-level stat growth should be applied externally.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerStatData.generated.h"

USTRUCT(BlueprintType)
struct RETRI_API FPlayerStatInfo
{
	GENERATED_BODY()

	// ---------------------- Currency ----------------------

	// 상점 / 성소 / 분해의 성소 등 주 화폐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Gold = 0;

	// 빛나는 우물에서 기억 강화에 소비
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DreamDust = 0;

	// ---------------------- Combat Stats ----------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 220.f;

	// 기본 공격 피해량 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower = 43.f;

	// 대부분의 기억/정수 피해량 계수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpellPower = 41.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 520.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense = 0.f;

	// 0.0 ~ 1.0
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CritRate = 0.05f;

	// 치명타 배율 (2.0 = 200%)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CritMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProjectileSpeed = 1200.f;

	// 초당 기본 공격 횟수 (합연산)
	// 최종 공격 속도 = Base + Base * (모든 보너스 합산)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed = 1.41f;

	// 기억 가속 (0 이상, 상한 없음)
	// 쿨다운 배율: 100 / (100 + MemoryAcceleration)
	// ex) 100 → 1/2, 300 → 1/4
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MemoryAcceleration = 0.f;

	// 화상 피해 보너스 배율 (0.0 = 보너스 없음)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BurnDamageBonus = 0.f;

	// ---------------------- Dash ----------------------

	// 보유 대시 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DashCount = 1;

	// 대시 기본 쿨다운 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashCooldown = 5.f;

	// ---------------------- Getters ----------------------

	int32 GetGold() const { return Gold; }
	int32 GetDreamDust() const { return DreamDust; }
	float GetMaxHP() const { return MaxHP; }
	float GetAttackPower() const { return AttackPower; }
	float GetSpellPower() const { return SpellPower; }
	float GetMoveSpeed() const { return MoveSpeed; }
	float GetDefense() const { return Defense; }
	float GetCritRate() const { return CritRate; }
	float GetCritMultiplier() const { return CritMultiplier; }
	float GetAttackRange() const { return AttackRange; }
	float GetProjectileSpeed() const { return ProjectileSpeed; }
	float GetAttackSpeed() const { return AttackSpeed; }
	float GetMemoryAcceleration() const { return MemoryAcceleration; }
	float GetBurnDamageBonus() const { return BurnDamageBonus; }
	int32 GetDashCount() const { return DashCount; }
	float GetDashCooldown() const { return DashCooldown; }

	// ---------------------- Setters ----------------------

	void AddGold(int32 Delta) { Gold = FMath::Max(0, Gold + Delta); }
	void AddDreamDust(int32 Delta) { DreamDust = FMath::Max(0, DreamDust + Delta); }
	void AddMaxHP(float Delta) { MaxHP += Delta; }
	void AddAttackPower(float Delta) { AttackPower += Delta; }
	void AddSpellPower(float Delta) { SpellPower += Delta; }
	void AddMoveSpeed(float Delta) { MoveSpeed += Delta; }
	void AddDefense(float Delta) { Defense = FMath::Max(0.f, Defense + Delta); }
	void AddCritRate(float Delta) { CritRate = FMath::Clamp(CritRate + Delta, 0.f, 1.f); }
	void AddCritMultiplier(float Delta) { CritMultiplier += Delta; }
	void AddAttackRange(float Delta) { AttackRange += Delta; }
	void AddProjectileSpeed(float Delta) { ProjectileSpeed += Delta; }
	void AddAttackSpeed(float Delta) { AttackSpeed = FMath::Max(0.1f, AttackSpeed + Delta); }

	void AddMemoryAcceleration(float Delta)
	{
		MemoryAcceleration = FMath::Max(0.f, MemoryAcceleration + Delta);
	}

	void AddBurnDamageBonus(float Delta) { BurnDamageBonus += Delta; }
	void AddDashCount(int32 Delta) { DashCount = FMath::Max(0, DashCount + Delta); }
	void AddDashCooldown(float Delta) { DashCooldown = FMath::Max(0.f, DashCooldown + Delta); }

	// ---------------------- Formulas ----------------------

	// 기억 가속 적용 실제 쿨다운
	// 공식: BaseCooldown * (100 / (100 + MemoryAcceleration))
	float ApplyCooldown(float BaseCooldown) const
	{
		return BaseCooldown * (100.f / (100.f + MemoryAcceleration));
	}

	// 방어력 적용 실제 피해 배율
	// 공식: 1 / (1 + Defense * 0.01)
	// 음수 방어력 없음
	float ApplyDefense(float IncomingDamage) const
	{
		return IncomingDamage / (1.f + Defense * 0.01f);
	}

	// 치명타 RNG 포함 최종 공격력 기반 데미지
	float CalcAttackDamage(float DamageRatio = 1.f) const
	{
		const float Raw = AttackPower * DamageRatio;
		const bool bCrit = FMath::FRand() < CritRate;
		return bCrit ? Raw * CritMultiplier : Raw;
	}

	// 주문력 기반 데미지 (대부분의 기억/스킬)
	float CalcSpellDamage(float DamageRatio = 1.f) const
	{
		const float Raw = SpellPower * DamageRatio;
		const bool bCrit = FMath::FRand() < CritRate;
		return bCrit ? Raw * CritMultiplier : Raw;
	}

	// 강제 치명타 데미지 (샐러맨더 가루 4타 등)
	float CalcForcedCritDamage(float DamageRatio = 1.f) const
	{
		return AttackPower * DamageRatio * CritMultiplier;
	}

	// ---------------------- Currency Spend ----------------------

	bool SpendGold(int32 Cost)
	{
		if (Gold < Cost) return false;
		Gold -= Cost;
		return true;
	}

	bool SpendDreamDust(int32 Cost)
	{
		if (DreamDust < Cost) return false;
		DreamDust -= Cost;
		return true;
	}
};
