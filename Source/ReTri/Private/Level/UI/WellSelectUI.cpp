

#include "Level/UI/WellSelectUI.h"

#include "Components/VerticalBox.h"
#include "Level/UI/WellSelectButtonUI.h"

UWellSelectButtonUI* UWellSelectUI::AddButton(FString Description, FString Cost, int32 Index)
{
	if (!ButtonClass) 
	{
		return nullptr;
	}
	
	UWellSelectButtonUI* Button = CreateWidget<UWellSelectButtonUI>(this, ButtonClass);
	Button->ClearFlags(RF_Transactional); 
	
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
