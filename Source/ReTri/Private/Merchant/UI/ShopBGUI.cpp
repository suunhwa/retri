

#include "Merchant/UI/ShopBGUI.h"

#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Merchant/UI/ShopSlotUI.h"

UShopSlotUI* UShopBGUI::AddButton(int32 Number, UTexture2D* ItemIcon, int32 GoldText, FLinearColor Color)
{
	if (!ShopSlotUIClass)
	{
		return nullptr;
	}
	
	UShopSlotUI* ShopSlot = CreateWidget<UShopSlotUI>(this, ShopSlotUIClass);
	ShopSlot->ClearFlags(RF_Transactional); 
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
