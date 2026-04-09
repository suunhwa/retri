// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/FloatingUI.h"

#include "Components/TextBlock.h"
#include "ReTri/ReTri.h"

void UFloatingUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFloatingUI::PlayScale(FText Text, FLinearColor OutLinearColor)
{
//	ShowText->SetText(FText::FromString(Text));
	ShowText->SetText(Text);
	FSlateFontInfo FontInfo = ShowText->GetFont();
	FontInfo.OutlineSettings.OutlineColor = OutLinearColor;
	FontInfo.OutlineSettings.OutlineSize = 2;
	ShowText->SetFont(FontInfo);
	
	PlayAnimation(Scale);
}

void UFloatingUI::PlayFloating(FText Text, FLinearColor OutLinearColor)
{
	//ShowText->SetText(FText::FromString(Text));
	ShowText->SetText(Text);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *Text.ToString());
	FSlateFontInfo FontInfo = ShowText->GetFont();
	FontInfo.OutlineSettings.OutlineColor = OutLinearColor;
	FontInfo.OutlineSettings.OutlineSize = 2;
	ShowText->SetFont(FontInfo);
	
	PlayAnimation(Floating);
}

void UFloatingUI::PlayFloatingLong(FText Text, FLinearColor OutLinearColor)
{
	//ShowText->SetText(FText::FromString(Text));
	ShowText->SetText(Text);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *Text.ToString());
	FSlateFontInfo FontInfo = ShowText->GetFont();
	FontInfo.OutlineSettings.OutlineColor = OutLinearColor;
	FontInfo.OutlineSettings.OutlineSize = 2;
	ShowText->SetFont(FontInfo);
	
	PlayAnimation(FloatingLong);
}
