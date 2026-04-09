// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "MerchantData.generated.h" 

USTRUCT(BlueprintType)
struct FMerchantDialogueRow : public FTableRowBase 
{
	GENERATED_BODY()

public:
	// 조금 전에 UI 출력용으로 FText를 쓰기로 하셨으니, FText로 세팅했습니다!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Merchant|Dialogue")
	FString Dialogue;
};
