// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/GoodsBase.h"
#include "GoodsGoldCoin.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API AGoodsGoldCoin : public AGoodsBase
{
	GENERATED_BODY()
	
public:
	AGoodsGoldCoin();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Gold = 10;
};
