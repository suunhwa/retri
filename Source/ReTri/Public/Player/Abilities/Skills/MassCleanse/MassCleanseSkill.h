#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "MassCleanseSkill.generated.h"

class AMassCleanseAoE;
class UNiagaraSystem;

/**
 * 대규모 정화 (획득 스킬)
 * 플레이어 주변 AoE로 빛 피해 + 플레이어 체력 회복
 * 쿨타임: 10초 / 피해: AP × 3.0 / 회복: HealAmount
 */
UCLASS()
class RETRI_API UMassCleanseSkill : public UAbilityBase
{
	GENERATED_BODY()

public:
	UMassCleanseSkill();

protected:
	virtual void Activate(ACharacter* Owner) override;

public:
	UPROPERTY(EditDefaultsOnly, Category="MassCleanse")
	TSubclassOf<AMassCleanseAoE> CleanseAoEClass;

	// 스킬 사용 시 플레이어에게 즉시 회복되는 체력
	UPROPERTY(EditDefaultsOnly, Category="MassCleanse")
	float HealAmount = 80.f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> CastEffect;

	// 플레이어 몸에 붙는 힐 이펙트 (체력 회복 중 표시)
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> HealEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	float CastEffectDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> CastSound;
};
