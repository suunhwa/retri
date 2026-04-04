// Fill out your copyright notice in the Description page of Project Settings.


#include "Merchant/UI/ShopBGUI.h"

#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Merchant/UI/ShopSlotUI.h"

UShopSlotUI* UShopBGUI::AddButton(int32 Number, UTexture2D* ItemIcon, int32 GoldText, FLinearColor Color)
{
	if (!ShopSlotUIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopSlotUI: UShopSlotUI가 설정되지 않았습니다."));
		return nullptr;
	}
	
	UShopSlotUI* ShopSlot = CreateWidget<UShopSlotUI>(this, ShopSlotUIClass);
	ShopSlot->ClearFlags(RF_Transactional); // PIE World Leak 방지
	ShopSlot->SetItemIcon(ItemIcon);
	ShopSlot->SetGoldText(GoldText, Color);
	ShopSlot->SlotNumber = Number;
	
	ItemWrapBox->AddChildToWrapBox(ShopSlot);
	
	return ShopSlot;
}

void UShopBGUI::ClearButtons()
{
	ItemWrapBox->ClearChildren();
}
