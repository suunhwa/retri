// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReTriGameData.h"
#include "Engine/GameInstance.h"
#include "ReTriGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API UReTriGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	UFUNCTION()
	UDataTable* GetInteractionData() const { return InteractionData; }
	
//! 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Interaction")
	UDataTable* InteractionData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	FPlayerStats SavePlayerStats;
	
//! 함수

};
