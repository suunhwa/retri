// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "DashAbility.generated.h"

class ADashGhostActor;

UCLASS()
class RETRI_API UDashAbility : public UAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void Activate(ACharacter* Owner) override;

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

private:
	void SpawnGhost();

	FTimerHandle GhostSpawnTimer;
	int32 GhostSpawnedCount = 0;
	TWeakObjectPtr<ACharacter> GhostOwner;
};
