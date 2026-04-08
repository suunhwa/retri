/**
 * 획득 스킬 - 동전 폭발 (Coin Explosion)
 *
 * 현재 골드의 1%를 소모해 동전을 투척.
 * 처음 적중한 적 위치에서 AoE 폭발, 빛 피해 + 1초 기절.
 * 피해량 = GoldSpent × ExplosionDamageCoeff (기본 1.5)
 *
 * 폭발 후 50/50 판정:
 *   성공 → ExplosionDamageCoeff 영구 2배
 *   실패 → ExplosionDamageCoeff 기본값(1.5)으로 리셋
 */

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "CoinExplosionSkill.generated.h"

class ACoinProjectile;

UCLASS()
class RETRI_API UCoinExplosionSkill : public UAbilityBase
{
	GENERATED_BODY()

public:
	UCoinExplosionSkill();

protected:
	virtual bool CanActivate(ACharacter* Owner) override;
	virtual void Activate(ACharacter* Owner) override;
	virtual bool ShouldRotateToMouse() const override { return false; }

private:
	// 폭발 완료 후 50/50 판정
	void RollExplosionMultiplier();

	// 폭발 피해 배율 (영구 누적 또는 리셋)
	UPROPERTY(EditDefaultsOnly, Category="CoinExplosion")
	float ExplosionDamageCoeff = 1.5f;

	// 배율 리셋 기준값
	UPROPERTY(EditDefaultsOnly, Category="CoinExplosion")
	float BaseExplosionDamageCoeff = 1.5f;

public:
	UPROPERTY(EditDefaultsOnly, Category="CoinExplosion")
	TSubclassOf<ACoinProjectile> CoinProjectileClass;

	// 현재 폭발 배율 조회 (UI 등에서 사용)
	UFUNCTION(BlueprintPure, Category="CoinExplosion")
	float GetExplosionDamageCoeff() const { return ExplosionDamageCoeff; }
};
