// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractCurse.h"

#include "MapSubSystem.h"
#include "NiagaraComponent.h"
#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"
#include "ReTri/ReTri.h"
#include "ReTriGameInstance.h"
#include "Kismet/GameplayStatics.h"


AInteractCurse::AInteractCurse()
{
	// ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/Free_Magic/VFX_Niagara/NS_Curse.NS_Curse'"));
	// if (TempNiagara.Succeeded()) NiagaraComp->SetAsset(TempNiagara.Object);
	// NiagaraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	// NiagaraComp->bAutoActivate = false;
}

void AInteractCurse::BeginPlay()
{
	Super::BeginPlay();
	
	
	//InteractableType = EInteractableType::Curse;
}

void AInteractCurse::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// GC에서 해결 되지 않고 플레이를 끈 경우 
	if (SelectUIInstance)
	{
		SelectUIInstance->ClearButtons();
		if (SelectUIInstance->IsInViewport())
		{
			SelectUIInstance->RemoveFromParent();
		}
		SelectUIInstance = nullptr;
	}
	
	Super::EndPlay(EndPlayReason);
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
		NewQuest.CurseColor = CurseData->CurseColor;
		
		MapSub->AddCurseQuest(NewQuest);
		
		JIWONLOG("저주 퀘스트: %s", *QuestMessage);
	}
	
	//todo InfoUI 띄우기 
	// SCREENLOG("%s", *CurseRewardDatas[RandomReward]->Info);
	// JIWONLOG("%s", *CurseRewardDatas[RandomReward]->Info);
	
	HideSelectUI();
}

void AInteractCurse::ShowSelectUI()
{
	Super::ShowSelectUI();
	
	if (!SelectUIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Select UI Class 할당 안됨"));
		return;
	}
	
	// 최소한 한 번만 생성, 이후에는 인스턴스 재사용
	if (!SelectUIInstance)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		SelectUIInstance = CreateWidget<USelectUI>(PC, SelectUIClass);
		if (SelectUIInstance)
		{
			// PIE Undo Buffer Leaks 방지
			// 위 코드는 위젯을 생성하자마자 
			// "이 위젯은 실행 취소 기록에 남기지 마! 넌 그냥 게임 플레이 도중에 쓰고 버릴 일회용이야!" 
			// 라고 플래그(RF_Transactional)를 강제로 지워버리는 역할
			SelectUIInstance->ClearFlags(RF_Transactional);
		}
	}

	if (!SelectUIInstance) return;

	// 이전 버튼들 정리 
	SelectUIInstance->ClearButtons();
	if (!SelectUIInstance->IsInViewport())
	{
		SelectUIInstance->AddToViewport();
	}
}

void AInteractCurse::HideSelectUI()
{
	Super::HideSelectUI();
	
	if (SelectUIInstance->IsInViewport())
	{
		SelectUIInstance->RemoveFromParent();
	}
}
