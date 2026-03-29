// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "PillarOfFireSkill.generated.h"


class APillarOfFireAoE;
class UNiagaraSystem;

/**
 * W 또는 E 획득 스킬 - 불기둥 (Pillar of Fire)
 * 마우스 지점에 불기둥을 소환
 * 즉발: AP × 2.5 (범위 화염 + 경직) / DoT: AP × 2.75 / 2.4초
 */
UCLASS()
class RETRI_API UPillarOfFireSkill : public UAbilityBase
{
	GENERATED_BODY()

public:
	UPillarOfFireSkill();

protected:
	virtual void Activate(ACharacter* Owner) override;

public:
	UPROPERTY(EditDefaultsOnly, Category="PillarOfFire")
	TSubclassOf<APillarOfFireAoE> PillarAoEClass;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> CastEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	float EffectDuration = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> CastSound;
};
