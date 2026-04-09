// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireOrbProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;

/**
 * 불기둥 스킬 발사체 - 순수 비주얼 전용
 * 타겟으로 날아간 뒤 사라짐. AoE 소환은 PillarOfFlameSkill에서 처리.
 */
UCLASS()
class RETRI_API AFireOrbProjectile : public AActor
{
	GENERATED_BODY()

public:
	AFireOrbProjectile();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MoveComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> OrbEffectComp;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> OrbEffect;
};
