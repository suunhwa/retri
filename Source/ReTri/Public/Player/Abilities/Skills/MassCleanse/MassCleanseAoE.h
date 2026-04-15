#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MassCleanseAoE.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;
class UCameraShakeBase;

/**
 * 대규모 정화 AoE 액터
 * 플레이어 위치 기준 구체 범위 내 적에게 즉발 빛 피해 적용
 * DoT 없음, 원소 효과 메커니즘 생략
 */
UCLASS()
class RETRI_API AMassCleanseAoE : public AActor
{
	GENERATED_BODY()

public:
	AMassCleanseAoE();

protected:
	virtual void BeginPlay() override;

public:
	// SpawnActorDeferred 직후 호출
	void Init(float InAbilityPower, AController* InInstigator);

private:
	void ApplyHit();
	void FinishEffect();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> HitVolume;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> CleanseEffect;

	float AbilityPower = 0.f;

	// 즉발 피해 계수 (AP × 3.0 = 300%)
	UPROPERTY(EditDefaultsOnly, Category="MassCleanse")
	float DamageCoeff = 3.0f;

	// AoE 반경
	UPROPERTY(EditDefaultsOnly, Category="MassCleanse")
	float HitRadius = 250.f;

	// 이펙트 지속 후 액터 소멸까지의 시간 (저지 불가 지속 시간과 동일)
	UPROPERTY(EditDefaultsOnly, Category="MassCleanse")
	float EffectDuration = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UCameraShakeBase> ImpactCS;

	TWeakObjectPtr<AController> InstigatorController;

	FTimerHandle LifeTimerHandle;

	TArray<TWeakObjectPtr<AActor>> GetEnemiesInRange() const;
};
