

#include "Level/UI/CurseQuestUI.h"

#include "MapSubSystem.h"
#include "ReTriGameInstance.h"
#include "Components/BackgroundBlur.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/Font.h"


void UCurseQuestUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetGameInstance()))
	{
		if (UMapSubSystem* MapSub = GI->GetSubsystem<UMapSubSystem>())
		{
			MapSub->OnCurseQuestChanged.AddDynamic(this, &UCurseQuestUI::UpdateCurseQuestText);
            
			UpdateCurseQuestText();
		}
	}
}

void UCurseQuestUI::UpdateCurseQuestText()
{
	if (UMapSubSystem* MapSub = GetGameInstance()->GetSubsystem<UMapSubSystem>())
	{
		QuestVerticalBox->ClearChildren();
		
		for (auto Quest : MapSub->ActiveCurseQuests)
		{
			if (MapSub->ActiveCurseQuests.IsEmpty())
			{
				if (BackBlur) BackBlur->SetVisibility(ESlateVisibility::Collapsed);
				return;
			}
		
			if (BackBlur) BackBlur->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			
			UTextBlock* TB = NewObject<UTextBlock>(this);
			FString QuestMessage;
			if (Quest.QuestType == EActiveCurseQuest::KillMinions)
				QuestMessage = FString::Printf(TEXT("몬스터를 %d	마리 처치하세요! (%d/%d)"), Quest.TargetCount, Quest.CurCount, Quest.TargetCount);
			else
				QuestMessage = FString::Printf(TEXT("맵을 %d개 클리어하세요! (%d/%d)"), Quest.TargetCount, Quest.CurCount, Quest.TargetCount);
			
			TB->SetText(FText::FromString(QuestMessage));
			FSlateFontInfo FontInfo = TB->GetFont();
			FontInfo.FontObject = CustomFontAsset;
			FontInfo.Size = 20.f;
			FontInfo.OutlineSettings.OutlineColor = Quest.CurseColor;
			FontInfo.OutlineSettings.OutlineSize = 2;
			TB->SetFont(FontInfo);
			
			QuestVerticalBox->AddChildToVerticalBox(TB);
		}
	}
}
