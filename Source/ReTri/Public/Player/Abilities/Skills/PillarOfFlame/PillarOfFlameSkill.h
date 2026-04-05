// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "PillarOfFlameSkill.generated.h"


class APillarOfFlameAoE;
class AFireOrbProjectile;
class UNiagaraSystem;

/**
 * W 또는 E 획득 스킬 - 불기둥 (Pillar of Fire)
 * 마우스 지점에 불기둥을 소환
 * 즉발: AP × 2.5 (범위 화염 + 경직) / DoT: AP × 2.75 / 2.4초
 */
UCLASS()
class RETRI_API UPillarOfFlameSkill : public UAbilityBase
{
	GENERATED_BODY()

public:
	UPillarOfFlameSkill();

protected:
	virtual void Activate(ACharacter* Owner) override;

public:
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	TSubclassOf<APillarOfFlameAoE> PillarAoEClass;

	// 날아가는 Orb 발사체 클래스
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFlame")
	TSubclassOf<AFireOrbProjectile> FireOrbClass;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> CastEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	float EffectDuration = 2.0f;

	// 손에 붙는 Orb 이펙트
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> OrbEffect;

	// Orb 유지 시간 (초)
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	float OrbDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> CastSound;
};
