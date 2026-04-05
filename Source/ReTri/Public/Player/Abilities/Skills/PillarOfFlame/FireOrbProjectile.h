// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireOrbProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class APillarOfFlameAoE;

/**
 * 불기둥 스킬 발사체
 * 손에서 타겟으로 날아가고, 도착하면 PillarOfFlameAoE 소환
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
	void Init(float InAbilityPower, AController* InInstigator, TSubclassOf<APillarOfFlameAoE> InAoEClass);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MoveComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> OrbEffectComp;

	// Orb 비주얼 이펙트
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> OrbEffect;

	// 착지 시 이펙트
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	float AbilityPower = 0.f;
	TWeakObjectPtr<AController> InstigatorController;
	TSubclassOf<APillarOfFlameAoE> AoEClass;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SpawnPillar();
};
