// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/LootBase.h"
#include "LootDreamPowderPillar.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API ALootDreamPowderPillar : public ALootBase
{
	GENERATED_BODY()
	
public:
	ALootDreamPowderPillar();
	
	virtual void Break() override;
};
