// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpBar.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class RETRI_API UExpBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// statcomponent.onexpchanged에 바인딩
	UFUNCTION()
	void OnExpChanged(int32 CurrentExp, int32 RequiredExp, int32 CurrentLevel);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ExpBar;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> LevelTxt;
	
};
