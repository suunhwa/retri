// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	
//! 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NZW|Interaction")
	UDataTable* InteractionData;
};
