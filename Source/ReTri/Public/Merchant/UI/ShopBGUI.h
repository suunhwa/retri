// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopBGUI.generated.h"

class UImage;
class UWrapBox;
class UShopSlotUI;

UCLASS()
class RETRI_API UShopBGUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UWrapBox* ItemWrapBox;
	
	UPROPERTY(EditAnywhere, Category="MapUI")
	TSubclassOf<UShopSlotUI> ShopSlotUIClass;
	
	// t	
	UFUNCTION(BlueprintCallable)
	UShopSlotUI* AddButton(UTexture2D* ItemIcon, int32 GoldText, FLinearColor Color = FLinearColor(0.022000f, 0.002732f, 0.002428f, 1.000000f));
	
	UFUNCTION(BlueprintCallable)
	void ClearButtons();
};
