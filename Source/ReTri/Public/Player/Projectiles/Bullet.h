// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class RETRI_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 발사체
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* moveComp;
	
	// 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* collisionComp;
	
	// 외관
	UPROPERTY(VisibleAnywhere)  
	class UStaticMeshComponent* bodyMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float Damage = 10.f;
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	
	// 일반탄 히트 이펙트
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> ImpactSound;

	// 강화탄 히트 이펙트
	UPROPERTY(EditDefaultsOnly, Category="Effects|Enhanced")
	TObjectPtr<UParticleSystem> EnhancedImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category="Effects|Enhanced")
	TObjectPtr<USoundBase> EnhancedImpactSound;

public:
	void SetBulletDamage(float InDamage) { Damage = InDamage; }
	float GetBulletDamage() const { return Damage; }
	void SetEnhanced();
	
	private:
private:
	bool bIsEnhanced = false;
};
