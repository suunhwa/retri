// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopSlotUI.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillSlotClicked);

UCLASS()
class RETRI_API UShopSlotUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	// 외부에서 바인딩 할 Delegate
	UPROPERTY(BlueprintAssignable)
	FOnSkillSlotClicked OnSlotClicked;
	
	UFUNCTION()
	void OnClicked();
	
	UFUNCTION(BlueprintCallable)
	void SetGoldText(int32 Gold, FLinearColor Color = FLinearColor(0.8f, 0.8f, 0.5f, 1.0f));

	UFUNCTION(BlueprintCallable)
	void SetItemIcon(UTexture2D* Texture);
	
	UPROPERTY(meta=(BindWidget))
	UButton* ItemSlotButton;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GoldText;
};
