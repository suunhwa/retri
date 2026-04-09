// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayDataUI.generated.h"

class UTextBlock;

UCLASS()
class RETRI_API UPlayDataUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayDataTitleText;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlayDataText;
	
	UFUNCTION()
	void SetTexts(FString Title, int32 Data);
};
