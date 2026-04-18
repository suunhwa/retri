

#include "Item/UI/DropItemUI.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UDropItemUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetItemInteractCanvasVisibility(ESlateVisibility::Hidden);
}

void UDropItemUI::SetItemIcon(UTexture2D* Texture)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(Texture);
	Brush.SetImageSize(FVector2D(65.0f, 65.f));
	Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
	Brush.OutlineSettings.RoundingType = ESlateBrushRoundingType::HalfHeightRadius;
	
	ItemIcon->SetBrush(Brush);
}

void UDropItemUI::SetItemName(FString Text)
{
	ItemName->SetText(FText::FromString(Text));
}

void UDropItemUI::SetDecomposeBar(float CurDeltaTime)
{
	float Percent = CurDeltaTime / 100.f;
	DecomposeBar->SetPercent(Percent);
}

void UDropItemUI::SetItemInteractCanvasVisibility(ESlateVisibility Visible)
{
	ItemInteractCanvas->SetVisibility(Visible);
}
