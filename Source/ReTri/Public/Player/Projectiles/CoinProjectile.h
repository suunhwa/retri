// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UCameraShakeBase;

// 폭발 완료 후 스킬에 통보 (50/50 판정용)
DECLARE_DELEGATE(FOnCoinExploded);

/**
 * 동전 폭발 스킬의 투사체
 * - 마우스 방향으로 날아가 처음 적중하는 적 위치에서 AoE 폭발
 * - 폭발: 빛 피해 + 1초 기절
 * - 폭발 완료 후 OnCoinExploded 델리게이트 호출 (50/50 배율 판정)
 */
UCLASS()
class RETRI_API ACoinProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACoinProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	// 스킬에서 스폰 직후 호출
	void Init(float InDamage, AController* InInstigator, FOnCoinExploded InCallback);

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 폭발 처리: AoE 피해 + 기절
	void Explode(FVector ExplosionLocation);

	// 기절 해제 (1초 후 타이머)
	void UnstunEnemy(TWeakObjectPtr<AActor> WeakEnemy);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MoveComp;

	// 폭발 반경
	UPROPERTY(EditDefaultsOnly, Category="CoinExplosion")
	float ExplosionRadius = 250.f;

	// 기절 지속 시간
	UPROPERTY(EditDefaultsOnly, Category="CoinExplosion")
	float StunDuration = 1.f;

	// 비행 중 메시 회전 속도 (도/초)
	UPROPERTY(EditDefaultsOnly, Category="CoinExplosion")
	float SpinRate = 720.f;

	// 비행 중 VFX (스폰 후 붙어서 따라다님)
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> FlightEffect;

	// 폭발 VFX
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	// 폭발 SFX
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> ExplosionSound;

	// 코인 날아갈 때 SFX
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> FlightSound;

	// 폭발 시 카메라 셰이크
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UCameraShakeBase> ExplosionCS;

	// 비행 중 활성화된 Niagara 컴포넌트 (폭발 시 비활성화)
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> FlightEffectComp;

	float Damage = 0.f;
	bool bHasExploded = false;

	TWeakObjectPtr<AController> InstigatorController;
	FOnCoinExploded OnCoinExploded;
};
