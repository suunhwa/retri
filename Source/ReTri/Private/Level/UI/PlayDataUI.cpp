

#include "Level/UI/PlayDataUI.h"

#include "Components/TextBlock.h"

void UPlayDataUI::SetTexts(FString Title, int32 Data)
{
	PlayDataTitleText->SetText(FText::FromString(Title));
	PlayDataText->SetText(FText::AsNumber(Data));
}
