// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandCannonBullet.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class RETRI_API AHandCannonBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHandCannonBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> MoveComp;
	
	// 발사체 트레일
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> TrailComp;
	
public:
	// 근거리 판정 거리
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float NearRange = 300.f;
	
	// 근거리 피해
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float NearDamage = 40.f;
	
	// 원거리 피해
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float FarDamage = 20.f;
	
	// 근거리 넉백 강도
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockBackForce = 800.f;
	
	void SetDamage(float InNearDamage, float InFarDamage);
	
	// effects
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> TrailEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> ParticleEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> ImpactSound;
	
private:
	FVector SpawnLocation;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
