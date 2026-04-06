// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractCurse.h"

#include "MapSubSystem.h"
#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"
#include "ReTri/ReTri.h"
#include "ReTriGameInstance.h"
#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"


void AInteractCurse::BeginPlay()
{
	Super::BeginPlay();
	
	//InteractableType = EInteractableType::Curse;
}

void AInteractCurse::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	UE_LOG(jiwon, Warning, TEXT("3가지 선택지 UI 띄우기 -> 저주를 받고 스텟 레벨업"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	// FName KeyName = FName("Curse");
	// bool* FoundValue = GetGameInstance()->GetSubsystem<UMapSubSystem>()->GetCurMapData().SpawnInteractableRowNames.Find(KeyName);
	// if (FoundValue) *FoundValue = true; 
	
	SetIsUsed(true);
	
	if (!CurseDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Curse Data Table 할당 되지 않음"));
		return;
	}
	
	// Curse Data 가져오기
	CurseDatas.Empty();
	CurseDataTable->GetAllRows<FCurseData>(TEXT("Curse::Interact"), CurseDatas);
	
	ShowSelectUI();
	if (!SelectUIInstance) return;

	for (int i = 0; i < CurseDatas.Num(); i++)
	{
		USelectButtonUI* Button = SelectUIInstance->AddButton(CurseDatas[i]->CurseName, CurseDatas[i]->CurseInfo, i, CurseDatas[i]->CurseColor);
		if (Button)
		{
			Button->OnSelectClicked.AddDynamic(this, &AInteractCurse::OnCurseSelected);
		}
	}
}

void AInteractCurse::OnCurseSelected(int32 Index)
{
	FCurseData* CurseData = CurseDatas[Index];
	JIWONLOG("선택된 저주: %s", *CurseData->CurseName);
	
	EActiveCurseQuest PickedQuestType = EActiveCurseQuest::None;
	int32 PickedTargetCount = 0;
	FString QuestMessage;
	
	int32 RandomCurse = FMath::RandRange(0, 1);
	if (RandomCurse == 0)
	{
		// SCREENLOG("몬스터를 %d마리 처치하세요!", CurseData->CurseMonster);
		PickedQuestType = EActiveCurseQuest::KillMinions;
		PickedTargetCount = CurseData->CurseMonster;
		QuestMessage = FString::Printf(TEXT("몬스터를 %d마리 처치하세요!"), PickedTargetCount);
	}
	else
	{
		//SCREENLOG("맵을 %d구역 클리어하세요!", CurseData->CurseMap);
		PickedQuestType = EActiveCurseQuest::ClearMaps;
		PickedTargetCount = CurseData->CurseMap;
		QuestMessage = FString::Printf(TEXT("맵을 %d개 클리어하세요!"), PickedTargetCount);
	}
	SCREENLOG("%s", *QuestMessage);
	
	auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return;
	
	CurseRewardDatas.Empty();
	CurseRewardDataTable->GetAllRows<FCurseRewardData>(TEXT("Curse::Select"), CurseRewardDatas);
	
	int32 RandomReward = FMath::RandRange(0, CurseRewardDatas.Num() - 1);
	int32 RewardVal = CurseRewardDatas[RandomReward]->RewardLevels[static_cast<int32>(CurseData->CurseType)];
	
	if (UMapSubSystem* MapSub = GI->GetSubsystem<UMapSubSystem>())
	{
		FActiveCurseQuest NewQuest;
		NewQuest.QuestType = PickedQuestType;
		NewQuest.TargetCount = PickedTargetCount;
		NewQuest.CurCount = 0;
		NewQuest.RewardType = CurseRewardDatas[RandomReward]->RewardType;
		NewQuest.RewardValue = RewardVal;
		NewQuest.RewardInfo = CurseRewardDatas[RandomReward]->Info;
		
		MapSub->ActiveCurseQuests.Add(NewQuest);
		
		JIWONLOG("저주 퀘스트: %s", *QuestMessage);
	}
	
	//todo InfoUI 띄우기 
	// SCREENLOG("%s", *CurseRewardDatas[RandomReward]->Info);
	// JIWONLOG("%s", *CurseRewardDatas[RandomReward]->Info);
	
	HideSelectUI();
}
