// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PillarOfFlameAoE.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UCameraShakeBase;

/**
 * 불기둥 스킬이 소환하는 범위 Actor
 * - 즉발: AP × 2.5 + 경직
 * - DoT: AP × 2.75 / 2.4초
 */
UCLASS()
class RETRI_API APillarOfFlameAoE : public AActor
{
	GENERATED_BODY()

public:
	APillarOfFlameAoE();

protected:
	virtual void BeginPlay() override;

public:
	/** 스킬 발동 전 AbilityPower 주입 */
	void Init(float InAbilityPower, AController* InInstigator);

private:
	void ApplyInitialHit();
	void ApplyDoTTick();
	void FinishDoT();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> HitVolume;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> FireEffect;

	float AbilityPower = 0.f;

	// 즉발 계수
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	float ImmediateDamageCoeff = 2.5f;

	// DoT 총 피해 계수
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	float DoTDamageCoeff = 2.75f;

	// DoT 지속 시간 (초)
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	float DoTDuration = 2.4f;

	// DoT 틱 간격
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	float DoTTickInterval = 0.4f;

	// 경직 지속 시간
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	float StaggerDuration = 0.25f;

	// AoE 반경
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	float HitRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> ImpactSound;

	// 즉발 타격 시 카메라 셰이크
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UCameraShakeBase> ImpactCS;

	// DoT 중 적에게 붙는 불타는 이펙트
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> BurnEffect;

	TWeakObjectPtr<AController> InstigatorController;

	// 붙인 번 이펙트 추적 (제거용)
	TArray<TWeakObjectPtr<UNiagaraComponent>> BurnEffectComps;

	// DoT 종료 후 이펙트 페이드아웃 대기 시간
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	float EffectFadeOutDelay = 1.5f;

	FTimerHandle DoTTimerHandle;
	FTimerHandle LifeTimerHandle;

	float DoTDamagePerTick = 0.f;

	/** 현재 범위 내 적 수집 */
	TArray<TWeakObjectPtr<AActor>> GetEnemiesInRange() const;
};

