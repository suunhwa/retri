#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PiercingBullet.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class RETRI_API APiercingBullet : public AActor
{
	GENERATED_BODY()

public:
	APiercingBullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MoveComp;

	// 발사체 트레일 (Particle)
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> TrailComp;

	// 발사체 트레일 (Niagara)
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraTrailComp;

public:
	// 피해량
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float Damage = 10.f;

	// 관통 가능 최대 적 수
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	int32 MaxPierceCount = 3;

	// effects
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> TrailEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> NiagaraTrailEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> ParticleEffect;

	// 히트 스파크 (Niagara)
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> HitSparkEffect;

	// 히트 스파크 (Particle)
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> HitSparkParticle;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> HitSound;

	void SetDamage(float InDamage) { Damage = InDamage; }

private:
	// 이미 맞은 액터 추적 (중복 피해 방지)
	TArray<AActor*> HitActors;
	int32 PierceCount = 0;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp,
	               AActor* OtherActor,
	               UPrimitiveComponent* OtherComp,
	               int32 OtherBodyIndex,
	               bool bFromSweep,
	               const FHitResult& SweepResult);
};
