// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "QuickTriggerSkill.generated.h"

class APiercingBullet;
class UNiagaraSystem;

/**
* R 고유 스킬 - 빠른 손
* 최대 3명의 적에게 적중하는 관통탄을 지정한 방향으로 3발 발사합니다.
* 피해: [120% → 150% → 420%] / 14초 충전, 최대 3충전 (시작 시 2충전)
*/

UCLASS()
class RETRI_API UQuickTriggerSkill : public UAbilityBase
{
	GENERATED_BODY()
	
public:
	UQuickTriggerSkill();
	
protected:
	virtual bool CanActivate(ACharacter* Owner) override;
	virtual void Activate(ACharacter* Owner) override;
	
public:
	// 최대 충전 수 
	UPROPERTY(EditDefaultsOnly, Category="FastHands")
	int32 MaxCharges = 3;
	
	// 충전 1회 당 재충전 시간 (s)
	UPROPERTY(EditDefaultsOnly, Category="FastHands")
	float RechargeTime = 14.f;
	
	// 3발 발사 간격 (s)
	UPROPERTY(EditDefaultsOnly, Category="FastHands")
	float BaseFireDelay = 0.08f;
	
	// 3발 피해 배율 (1번탄: 120%, 2번탄: 150%, 3번탄: 420%)
	UPROPERTY(EditDefaultsOnly, Category="FastHands")
	TArray<float> DamageMultipliers = {1.2f, 1.5f, 4.2f};
	
	UPROPERTY(EditDefaultsOnly, Category="FastHands")
	TSubclassOf<APiercingBullet> PiercingBullet;
	
	// effects
	UPROPERTY(EditDefaultsOnly, Category="FastHands|Effects")
	TObjectPtr<UNiagaraSystem> FireEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="FastHands|Effects")
	TObjectPtr<USoundBase> FireSound;
	
	UFUNCTION(BlueprintPure, Category="FastHands")
	int32 GetCurrentCharges() const { return CurrentCharges; }
	
private:
	// 현재 충전 수
	int32 CurrentCharges = 2;
	
	TArray<FTimerHandle> RechargeTimerHandle;
	
	FTimerHandle FireTimerHandle2;
	FTimerHandle FireTimerHandle3;
	
	void FireShot(TWeakObjectPtr<ACharacter> WeakOwner, int32 ShotIndex, FVector Direction, float BaseDamage);
	
	void RestoreCharge();
};
