// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "AntiGravitySkill.generated.h"

class AAntiGravityAOE;
class UNiagaraSystem;

/**
 * W 또는 E 획득 스킬 - 중력 반전 (Anti-Gravity)
 * 마우스 지점의 중력을 반전
 * 적을 1.6초 부양 후 착지 시 AP × 4.0 피해 + 기절 효과
 */
UCLASS()
class RETRI_API UAntiGravitySkill : public UAbilityBase
{
	GENERATED_BODY()

public:
	UAntiGravitySkill();

protected:
	virtual void Activate(ACharacter* Owner) override;

public:
	UPROPERTY(EditDefaultsOnly, Category="GravityReversal")
	TSubclassOf<AAntiGravityAOE> AntiGravityAoEClass;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> CastEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	float EffectDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> CastSound;
};

