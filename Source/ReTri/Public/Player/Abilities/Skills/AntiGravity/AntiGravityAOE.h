// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AntiGravityAOE.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

/**
 * 중력반전 스킬이 소환하는 범위 Actor
 * - 즉발: 범위 내 적 띄우기 (1.6초)
 * - 착지: AP × 4.0
 */
UCLASS()
class RETRI_API AAntiGravityAOE : public AActor
{
	GENERATED_BODY()

public:
	AAntiGravityAOE();

protected:
	virtual void BeginPlay() override;

public:
	void Init(float InAbilityPower, AController* InInstigator);

private:
	void ApplyLift();
	void ApplyLandingDamage();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> HitVolume;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> GravityEffect;

	float AbilityPower = 0.f;

	// 착지 피해 계수
	UPROPERTY(EditDefaultsOnly, Category="GravityReversal")
	float LandingDamageCoeff = 4.0f;

	// 부양 지속 시간
	UPROPERTY(EditDefaultsOnly, Category="GravityReversal")
	float LiftDuration = 1.6f;

	// 위로 띄우는 속도
	UPROPERTY(EditDefaultsOnly, Category="GravityReversal")
	float LaunchZVelocity = 100.f;

	// AoE 반경
	UPROPERTY(EditDefaultsOnly, Category="GravityReversal")
	float HitRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> LiftSound;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> LandSound;

	TWeakObjectPtr<AController> InstigatorController;

	FTimerHandle LandingTimerHandle;

	/** 띄운 적 목록 (착지 피해에 사용) */
	TArray<TWeakObjectPtr<AActor>> LiftedEnemies;
};
