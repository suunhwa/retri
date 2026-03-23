// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "DashAbility.generated.h"

UCLASS()
class RETRI_API UDashAbility : public UAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void Activate(ACharacter* Owner) override;

	// 대시 순간 가해지는 속도 (값 올릴수록 더 멀리 감)
	UPROPERTY(EditDefaultsOnly, Category="Dash")
	float DashImpulse = 5000.f;
};
