// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReTriGameInstance.h"
#include "Components/ActorComponent.h"
#include "Player/Data/PlayerStatData.h"
#include "Player/Data/PlayerLevelGrowthData.h"
#include "StatComponent.generated.h"

// 성소/강화에서 올릴 수 있는 스탯 종류
UENUM(BlueprintType)
enum class EStatTypes : uint8
{
	Gold,
	DreamDust,
	MaxHP,
	AttackPower,
	SpellPower,
	AttackSpeed,
	CritRate,
	CritMultiplier,
	MoveSpeed,
	Defense,
	MemoryAcceleration,
	AttackRange,
	ProjectileSpeed,
	DashCount,
	DashCooldown,
	BurnDamageBonus,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, EStatTypes, StatType, float, NewValue);
// CurrentExp, 다음 레벨까지 필요한 누적 경험치, 현재 레벨
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnExpChanged, int32, CurrentExp, int32, RequiredExp, int32, CurrentLevel);

UCLASS(ClassGroup=(Player), meta=(BlueprintSpawnableComponent))
class RETRI_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	// GameInstance.CurPlayerStats 갱신
	void SyncToGameInstance();

	// 여러 스탯을 한 번에 올릴 때 배치 시작 — EndBatch 호출 전까지 Sync 지연
	UFUNCTION(BlueprintCallable, Category="Stats")
	void BeginBatch();

	// 배치 종료 — Sync 한 번 실행
	UFUNCTION(BlueprintCallable, Category="Stats")
	void EndBatch();

	// 성소 등 외부에서 스탯 강화 시 호출 — Added에 누적되어 레벨업 후에도 유지
	UFUNCTION(BlueprintCallable, Category="Stats")
	void ApplyStatModifier(EStatTypes Type, float Delta);

	// 골드 차감 — 잔액 부족 시 false 반환
	UFUNCTION(BlueprintCallable, Category="Stats|Currency")
	bool SpendGold(int32 Cost);

	// 드림더스트 차감 — 잔액 부족 시 false 반환
	UFUNCTION(BlueprintCallable, Category="Stats|Currency")
	bool SpendDreamDust(int32 Cost);

	// 레벨업 시 호출 — Base만 덮어씀, Added(성소) 보존
	UFUNCTION(BlueprintCallable, Category="Stats")
	void LoadStatsForLevel(int32 Level);
	
	void BroadcastLevelStatsChanged();

	// 해당 레벨 도달에 필요한 누적 경험치 (-1: 테이블 미설정 또는 범위 초과)
	UFUNCTION(BlueprintPure, Category="Stats")
	int32 GetRequiredExpForLevel(int32 Level) const;
	
	// 적 처치 등 외부에서 경험치 추가 시 호출 — 레벨업 자동 처리
	UFUNCTION(BlueprintCallable, Category="Stats")
	void AddExp(int32 Amount);

	UFUNCTION(BlueprintPure, Category="Stats")
	int32 GetCurrentExp() const { return CurrentExp; }
	
	UFUNCTION(BlueprintPure, Category="Stats")
	int32 GetTotalExp() const { return TotalExp; }

	// 성소 / 레벨 시스템에서 현재 스탯 전체를 읽을 때 사용
	UFUNCTION(BlueprintPure, Category="Stats")
	FPlayerStatInfo GetStatInfo() const;

	UFUNCTION(BlueprintPure, Category="Stats|Currency")
	int32 GetGold() const { return Gold; }

	UFUNCTION(BlueprintPure, Category="Stats|Currency")
	int32 GetDreamDust() const { return DreamDust; }

	// 최종값 = Base + Bonus
	UFUNCTION(BlueprintPure, Category="Stats")
	float GetMaxHP() const { return BaseMaxHP + AddedMaxHP; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetAttackPower() const { return BaseAttackPower + AddedAttackPower; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetSpellPower() const { return BaseSpellPower + AddedSpellPower; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetAttackSpeed() const { return FMath::Max(0.1f, BaseAttackSpeed * AddedAttackSpeed); }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetCritRate() const { return FMath::Clamp(BaseCritRate + AddedCritRate, 0.f, 1.f); }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetCritMultiplier() const { return FMath::Max(1.f, BaseCritMultiplier + AddedCritMultiplier); }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetMoveSpeed() const { return BaseMoveSpeed + AddedMoveSpeed; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetDefense() const { return BaseDefense + AddedDefense; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetMemoryAcceleration() const { return BaseMemoryAcceleration + AddedMemoryAcceleration; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetAttackRange() const { return BaseAttackRange + AddedAttackRange; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetProjectileSpeed() const { return BaseProjectileSpeed + AddedProjectileSpeed; }

	UFUNCTION(BlueprintPure, Category="Stats")
	int32 GetDashCount() const { return BaseDashCount + AddedDashCount; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetDashCooldown() const { return FMath::Max(0.1f, BaseDashCooldown + AddedDashCooldown); }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetBurnDamageBonus() const { return BaseBurnDamageBonus + AddedBurnDamageBonus; }

	UFUNCTION(BlueprintPure, Category="Stats")
	int32 GetCurrentLevel() const { return CurrentLevel; }

	// 스탯 변경 시 브로드캐스트 — UI / 성소 / 레벨 시스템에서 구독
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnStatChanged OnStatChanged;
	
	// 경험치/레벨 변경 시 브로드캐스트 — ExpBar UI에서 구독
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnExpChanged OnExpChanged;

private:
	UPROPERTY()
	TObjectPtr<class UHealthComponent> HealthComp;

	bool bBatchMode = false;

	UPROPERTY(VisibleAnywhere, Category="Stats|Level", meta=(AllowPrivateAccess=true))
	int32 CurrentLevel = 1;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Level", meta=(AllowPrivateAccess=true))
	int32 CurrentExp = 0;

	UPROPERTY(VisibleAnywhere, Category="Stats|Level", meta=(AllowPrivateAccess=true))
	int32 TotalExp = 0;
	
	// ── Currency: 단순 누적 잔액 ──────────────────────────────────────────
	UPROPERTY(VisibleAnywhere, Category="Stats|Currency", meta=(AllowPrivateAccess=true))
	int32 Gold = 0;

	UPROPERTY(VisibleAnywhere, Category="Stats|Currency", meta=(AllowPrivateAccess=true))
	int32 DreamDust = 0;

	// ── Base: 레벨 테이블에서 설정, 레벨업 시 덮어씀 ──────────────────────
	// 기본값 
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseMaxHP = 220.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseAttackPower = 43.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseSpellPower = 41.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseAttackSpeed = 1.41f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseCritRate = 0.05f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseCritMultiplier = 2.0f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseMoveSpeed = 520.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseDefense = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseMemoryAcceleration = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseAttackRange = 1500.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseProjectileSpeed = 1200.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	int32 BaseDashCount = 1;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseDashCooldown = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float BaseBurnDamageBonus = 0.f;

	// ── Added: 성소/강화에서 누적, 레벨업 후에도 유지 ────────────────────
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedMaxHP = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedAttackPower = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedSpellPower = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedAttackSpeed = 1.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedCritRate = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedCritMultiplier = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedMoveSpeed = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedDefense = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedMemoryAcceleration = 1.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedAttackRange = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedProjectileSpeed = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	int32 AddedDashCount = 0;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedDashCooldown = 0.f;
	
	UPROPERTY(VisibleAnywhere, Category="Stats|Added", meta=(AllowPrivateAccess=true))
	float AddedBurnDamageBonus = 0.f;
};
