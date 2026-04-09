// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/SelectUI.h"

#include "Components/VerticalBox.h"
#include "Level/UI/SelectButtonUI.h"

USelectButtonUI* USelectUI::AddButton(FString Title, FString Info, int32 Index, FLinearColor Color)
{
	if (!ButtonClass) 
	{
		// UE_LOG(LogTemp, Warning, TEXT("SelectUI: ButtonClass가 설정되지 않았습니다."));
		return nullptr;
	}
	
	USelectButtonUI* Button = CreateWidget<USelectButtonUI>(this, ButtonClass);
	Button->ClearFlags(RF_Transactional); // PIE World Leak 방지
	
	Button->ThisIndex = Index;
	Button->SetTextTitle(Title, Color);
	Button->SetTextInfo(Info);
	
	VerticalBox->AddChildToVerticalBox(Button);
	
	return Button;
}

void USelectUI::ClearButtons()
{
	VerticalBox->ClearChildren();
}


