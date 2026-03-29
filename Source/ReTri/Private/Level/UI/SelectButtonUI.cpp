// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/SelectButtonUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Fonts/SlateFontInfo.h"

void USelectButtonUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ButtonSelect)
		ButtonSelect->OnClicked.AddDynamic(this, &USelectButtonUI::HandleClicked);
}

void USelectButtonUI::HandleClicked()
{
	OnSelectClicked.Broadcast(ThisIndex);
}

void USelectButtonUI::SetTextTitle(FString Title, FLinearColor Color)
{
	if (!TextTitle) return;
	TextTitle->SetText(FText::FromString(Title));
	FSlateFontInfo FontInfo = TextTitle->GetFont();
	FontInfo.OutlineSettings.OutlineColor = Color;
	FontInfo.OutlineSettings.OutlineSize = 2;
	TextTitle->SetFont(FontInfo);
}

void USelectButtonUI::SetTextInfo(FString Info)
{
	if (!TextInfo) return;
	TextInfo->SetText(FText::FromString(Info));
}
