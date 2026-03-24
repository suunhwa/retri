// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyData.h"
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
	
	
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Collision)
	class UBoxComponent* SwordCollision;
	
	
	
protected:
	UFUNCTION(BlueprintCallable, category=Collision)
	void SetSwordCollisionEnabled(bool bEnabled);
	
	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
	
};

