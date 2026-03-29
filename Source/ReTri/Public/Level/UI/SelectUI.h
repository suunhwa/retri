// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectUI.generated.h"

class UVerticalBox;
class USelectButtonUI;

/**
 * 
 */
UCLASS()
class RETRI_API USelectUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* VerticalBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USelectButtonUI> ButtonClass;
	
	UFUNCTION(BlueprintCallable)
	USelectButtonUI* AddButton(FString Title, FString Info, int32 Index, FLinearColor Color = FLinearColor(0.022000f, 0.002732f, 0.002428f, 1.000000f));
	
	UFUNCTION(BlueprintCallable)
	void ClearButtons();
};
