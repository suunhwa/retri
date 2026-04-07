// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/WellSelectUI.h"

#include "Components/VerticalBox.h"
#include "Level/UI/WellSelectButtonUI.h"

UWellSelectButtonUI* UWellSelectUI::AddButton(FString Description, FString Cost, int32 Index)
{
	if (!ButtonClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectUI: ButtonClass가 설정되지 않았습니다."));
		return nullptr;
	}
	
	UWellSelectButtonUI* Button = CreateWidget<UWellSelectButtonUI>(this, ButtonClass);
	Button->ClearFlags(RF_Transactional); // PIE World Leak 방지
	
	Button->ThisIndex = Index;
	Button->SetTextDescription(Description);
	Button->SetTextCost(Cost);
	
	VerticalBox->AddChildToVerticalBox(Button);
	
	return Button;
}

void UWellSelectUI::ClearButtons()
{
	VerticalBox->ClearChildren();
}
