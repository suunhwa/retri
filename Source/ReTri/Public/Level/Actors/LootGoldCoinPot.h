// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/LootBase.h"
#include "LootGoldCoinPot.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API ALootGoldCoinPot : public ALootBase
{
	GENERATED_BODY()
	
public:
	ALootGoldCoinPot();
	
	virtual void Break() override;
};
