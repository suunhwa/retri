// Fill out your copyright notice in the Description page of Project Settings.


#include "Merchant/UI/ShopBGUI.h"

#include "Merchant/UI/ShopSlotUI.h"

UShopSlotUI* UShopBGUI::AddButton(FString Title, FString Info, int32 Index, FLinearColor Color)
{
	if (!ShopSlotUIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopSlotUI: ButtonClass가 설정되지 않았습니다."));
		return nullptr;
	}
	
	UShopSlotUI* ShopSlot = CreateWidget<UShopSlotUI>(this, ShopSlotUIClass);
	//todo 여기 매개변수랑 이런거 바꿔야함
	//!! Slot->ItemIcon = ItemIcon;
	//!! Slot->GoldText = GoldText;
	return ShopSlot;
}

void UShopBGUI::ClearButtons()
{
}
