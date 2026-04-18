

#include "Level/UI/SelectUI.h"

#include "Components/VerticalBox.h"
#include "Level/UI/SelectButtonUI.h"

USelectButtonUI* USelectUI::AddButton(FString Title, FString Info, int32 Index, FLinearColor Color)
{
	if (!ButtonClass) 
	{
		return nullptr;
	}
	
	USelectButtonUI* Button = CreateWidget<USelectButtonUI>(this, ButtonClass);
	Button->ClearFlags(RF_Transactional);
	
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


