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
	void SetGoldText(FString Gold, FLinearColor Color = FLinearColor(0.022000f, 0.002732f, 0.002428f, 1.000000f));

	UPROPERTY(meta=(BindWidget))
	UButton* ItemSlotButton;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GoldText;
};
