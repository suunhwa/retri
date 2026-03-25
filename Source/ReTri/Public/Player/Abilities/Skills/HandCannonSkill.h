// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "HandCannonSkill.generated.h"

class AHandCannonBullet;
class UNiagaraSystem;
class UParticleSystem;

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
	
	// 이펙트 
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> HandCannonEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> MuzzleEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> FireSound;
};
