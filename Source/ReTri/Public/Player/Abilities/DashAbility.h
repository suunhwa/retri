// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "Sound/SoundBase.h"
#include "DashAbility.generated.h"

class ADashGhostActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDashChargeChanged, int32, Current, int32, Max);

UCLASS()
class RETRI_API UDashAbility : public UAbilityBase
{
	GENERATED_BODY()

public:
	UDashAbility();

	UPROPERTY(BlueprintAssignable, Category="Dash")
	FOnDashChargeChanged OnChargeChanged;

	UFUNCTION(BlueprintPure, Category="Dash")
	int32 GetCurrentCharges() const { return CurrentCharges; }

	UFUNCTION(BlueprintPure, Category="Dash")
	int32 GetMaxCharges() const { return MaxDashCharges; }

protected:
	virtual bool CanActivate(ACharacter* Owner) override;
	virtual void Activate(ACharacter* Owner) override;
	virtual bool ShouldRotateToMouse() const override { return false; }

	// 최대 충전 횟수
	UPROPERTY(EditDefaultsOnly, Category="Dash")
	int32 MaxDashCharges = 3;

	// 충전 1회당 회복 시간 (초)
	UPROPERTY(EditDefaultsOnly, Category="Dash")
	float DashRechargeTime = 3.3f;

	// 대시 순간 가해지는 속도 (값 올릴수록 더 멀리 감)
	UPROPERTY(EditDefaultsOnly, Category="Dash")
	float DashImpulse = 5000.f;

	// 잔상 Actor 클래스 (BP에서 할당)
	UPROPERTY(EditDefaultsOnly, Category="Dash|Ghost")
	TSubclassOf<ADashGhostActor> GhostActorClass;

	// 잔상 생성 간격 (초)
	UPROPERTY(EditDefaultsOnly, Category="Dash|Ghost")
	float GhostSpawnInterval = 0.02f;

	// 잔상 총 생성 개수
	UPROPERTY(EditDefaultsOnly, Category="Dash|Ghost")
	int32 GhostCount = 10;

	UPROPERTY(EditDefaultsOnly, Category="Dash|Sound")
	TObjectPtr<USoundBase> DashSound;

	UPROPERTY(EditDefaultsOnly, Category="Dash|Sound")
	TSubclassOf<UCameraShakeBase> DashCS;

private:
	void SpawnGhost();
	void RestoreCharge();

	int32 CurrentCharges = 3;
	TArray<FTimerHandle> RechargeHandles;

	FTimerHandle GhostSpawnTimer;
	int32 GhostSpawnedCount = 0;
	TWeakObjectPtr<ACharacter> GhostOwner;
};
