// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "DarkMoon.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API ADarkMoon : public AEnemyBase
{
	GENERATED_BODY()

	
public:
	ADarkMoon();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	
	
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Collision)
	class UBoxComponent* SwordCollision = nullptr;
	
private:
	bool bIsBattleStarted = false; // 전투가 이미 시작되었는지 체크
	
	FTimerHandle DeathTimerHandle;
	
	UFUNCTION()
	void DeathAnimFinished();
	
	
protected:
	UFUNCTION(BlueprintCallable, category=Collision)
	//void SetSwordCollisionEnabled(bool bEnabled);
	
	
	void StartBattleEvent();
	void ReduceBossHP();
	virtual void UpdatePhase() override;
	void BossDead();
	
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	UFUNCTION(BlueprintCallable, category=Collision)
	void OnSwordCollision();
	UFUNCTION(BlueprintCallable, category=Collision)
	void OffSwordCollision();
	
	
};

