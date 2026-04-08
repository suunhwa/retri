// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/WellSelectButtonUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWellSelectButtonUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ButtonSelect)
		ButtonSelect->OnClicked.AddUniqueDynamic(this, &UWellSelectButtonUI::OnClicked);
}

void UWellSelectButtonUI::NativeDestruct()
{
	if (ButtonSelect)
		ButtonSelect->OnClicked.RemoveDynamic(this, &UWellSelectButtonUI::OnClicked);
		
	Super::NativeDestruct();
}

void UWellSelectButtonUI::OnClicked()
{
	OnWellSelectClicked.Broadcast(ThisIndex);
}

void UWellSelectButtonUI::SetTextDescription(FString Title)
{
	if (!TextDescription) return;
	TextDescription->SetText(FText::FromString(Title));
}

void UWellSelectButtonUI::SetTextCost(FString Info)
{
	if (!TextCost) return;
	TextCost->SetText(FText::FromString(Info));
}
