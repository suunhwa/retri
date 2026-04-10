// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class RETRI_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* moveComp;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* collisionComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* bodyMesh;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float Damage = 10.f;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 일반탄
	UPROPERTY(EditDefaultsOnly, Category="Effects|Normal")
	TObjectPtr<UNiagaraSystem> TrailEffect;		// 비행 중 트레일

	UPROPERTY(EditDefaultsOnly, Category="Effects|Normal")
	TObjectPtr<UNiagaraSystem> HitEffect;		// 적 맞았을 때

	UPROPERTY(EditDefaultsOnly, Category="Effects|Normal")
	TObjectPtr<USoundBase> FireSound;			// 발사 소리

	UPROPERTY(EditDefaultsOnly, Category="Effects|Normal")
	TObjectPtr<USoundBase> HitSound;

	// 강화탄
	UPROPERTY(EditDefaultsOnly, Category="Effects|Enhanced")
	TObjectPtr<UNiagaraSystem> EnhancedTrailEffect;	// 강화탄 비행 중 트레일

	UPROPERTY(EditDefaultsOnly, Category="Effects|Enhanced")
	TObjectPtr<UNiagaraSystem> EnhancedHitEffect;	// 강화탄 적 맞았을 때

	UPROPERTY(EditDefaultsOnly, Category="Effects|Enhanced")
	TObjectPtr<USoundBase> EnhancedFireSound;	// 강화탄 발사 소리

	UPROPERTY(EditDefaultsOnly, Category="Effects|Enhanced")
	TObjectPtr<USoundBase> EnhancedHitSound;

public:
	void SetBulletDamage(float InDamage) { Damage = InDamage; }
	float GetBulletDamage() const { return Damage; }
	void SetEnhanced();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> TrailEffectComp;

	bool bIsEnhanced = false;
};
