#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "HandCannonSkill.generated.h"

class AHandCannonBullet;
class UNiagaraSystem;
class UParticleSystem;
class UCameraShakeBase;

UCLASS()
class RETRI_API UHandCannonSkill : public UAbilityBase
{
	GENERATED_BODY()

public:
	UHandCannonSkill();

protected:
	virtual void Activate(ACharacter* Owner) override;

public:
	UPROPERTY(EditDefaultsOnly, Category="HandCannon")
	TSubclassOf<AHandCannonBullet> HandCannonClass;

	// 플레이어 반동 강도
	UPROPERTY(EditDefaultsOnly, Category="HandCannon")
	float RecoilForce = 1200.f;

	// AbilityPower 기준 근/원거리 데미지 계수
	UPROPERTY(EditDefaultsOnly, Category="HandCannon")
	float NearCoefficient = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category="HandCannon")
	float FarCoefficient = 0.8f;

	// 이펙트 
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> HandCannonEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> FireSound;

	// 발사 + 반동 시 카메라 셰이크
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UCameraShakeBase> FireCS;
};
