// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "PillarOfFlameSkill.generated.h"

class APillarOfFlameAoE;
class UNiagaraSystem;

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

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> CastEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> CastSound;
};
