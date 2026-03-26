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
	class UBoxComponent* SwordCollision;
	

	
	
protected:
	UFUNCTION(BlueprintCallable, category=Collision)
	void SetSwordCollisionEnabled(bool bEnabled);
	
	
	void StartBattleEvent();
	void ReduceBossHP();
	void UpdatePhase();
	
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	UFUNCTION(BlueprintCallable, category=Collision)
	void OnSwordCollision();
	UFUNCTION(BlueprintCallable, category=Collision)
	void OffSwordCollision();
	
	
};

