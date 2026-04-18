

#include "Merchant/UI/ShopSlotUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Fonts/SlateFontInfo.h"
#include "Kismet/GameplayStatics.h"

void UShopSlotUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ItemSlotButton)
	{
		ItemSlotButton->OnClicked.AddDynamic(this, &UShopSlotUI::OnClicked);
		ItemSlotButton->OnHovered.AddDynamic(this, &UShopSlotUI::OnHover);
	}
}

void UShopSlotUI::OnClicked()
{
	OnSlotClicked.Broadcast(SlotNumber);
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
}

void UShopSlotUI::OnHover()
{
	UGameplayStatics::PlaySound2D(GetWorld(), HoverSound);
}

void UShopSlotUI::SetGoldText(int32 Gold, FLinearColor Color)
{
	if (!GoldText) return;
	GoldText->SetText(FText::AsNumber(Gold));
	FSlateFontInfo FontInfo = GoldText->GetFont();
	FontInfo.OutlineSettings.OutlineColor = Color;
	FontInfo.OutlineSettings.OutlineSize = 2;
	GoldText->SetFont(FontInfo);
}

void UShopSlotUI::SetItemIcon(UTexture2D* Texture)
{
	if (!ItemIcon) return;
	
	FSlateBrush Brush;
	Brush.SetResourceObject(Texture);
	Brush.SetImageSize(FVector2D(120.0f, 120.f));
	ItemIcon->SetBrush(Brush);
}
