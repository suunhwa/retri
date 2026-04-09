// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/CurseQuestUI.h"

#include "MapSubSystem.h"
#include "ReTriGameInstance.h"
#include "Components/BackgroundBlur.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/Font.h"

class UMapSubSystem;
class UReTriGameInstance;

void UCurseQuestUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetGameInstance()))
	{
		if (UMapSubSystem* MapSub = GI->GetSubsystem<UMapSubSystem>())
		{
			// 델리게이트 바인딩
			MapSub->OnCurseQuestChanged.AddDynamic(this, &UCurseQuestUI::UpdateCurseQuestText);
            
			// 최초 한 번 갱신 (이미 받아놓은 저주가 있을 수 있으므로)
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
			
			//CreateWidget 함수는 우리가 만든 블루프린트 위젯(UserWidget)을 띄울 때 사용. UTextBlock 같은 엔진 기본 컴포넌트(UWidget)를 C++에서 동적으로 생성할 때는 NewObject<UTextBlock>(this)
			UTextBlock* TB = NewObject<UTextBlock>(this);
			FString QuestMessage;
			if (Quest.QuestType == EActiveCurseQuest::KillMinions)
			{
				// SCREENLOG("몬스터를 %d마리 처치하세요!", CurseData->CurseMonster);
				QuestMessage = FString::Printf(TEXT("몬스터를 %d	마리 처치하세요! (%d/%d)"), Quest.TargetCount, Quest.CurCount, Quest.TargetCount);
			}
			else
			{
				//SCREENLOG("맵을 %d구역 클리어하세요!", CurseData->CurseMap);
				QuestMessage = FString::Printf(TEXT("맵을 %d개 클리어하세요! (%d/%d)"), Quest.TargetCount, Quest.CurCount, Quest.TargetCount);
			}
			
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
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("MapSub 안불러와짐"));
	// }
}
