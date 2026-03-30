// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"

#include "Minion.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API AMinion : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	AMinion();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 플레이어
	UPROPERTY()
	class ACharacter* PlayerTarget;
	
	// 미니언 공격 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAttacking = false;
	
	
protected:
	void CheckDistanceAndMove(float DeltaTime);
	
	
};
