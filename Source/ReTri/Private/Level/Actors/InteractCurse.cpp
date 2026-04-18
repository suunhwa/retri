

#include "Level/Actors/InteractCurse.h"

#include "MapSubSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"
#include "ReTri/ReTri.h"
#include "ReTriGameInstance.h"
#include "Kismet/GameplayStatics.h"


AInteractCurse::AInteractCurse()
{
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagara(TEXT("/Game/Free_Magic/VFX_Niagara/NS_Curse.NS_Curse"));
	if (TempNiagara.Succeeded()) NiagaraComp->SetAsset(TempNiagara.Object);
	NiagaraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	NiagaraComp->bAutoActivate = false;
}

void AInteractCurse::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractCurse::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
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
	
	SetIsUsed(true);
	
	if (!CurseDataTable) return;
	
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
	
	EActiveCurseQuest PickedQuestType = EActiveCurseQuest::None;
	int32 PickedTargetCount = 0;
	FString QuestMessage;
	
	int32 RandomCurse = FMath::RandRange(0, 1);
	if (RandomCurse == 0)
	{
		PickedQuestType = EActiveCurseQuest::KillMinions;
		PickedTargetCount = CurseData->CurseMonster;
		QuestMessage = FString::Printf(TEXT("몬스터를 %d마리 처치하세요!"), PickedTargetCount);
	}
	else
	{
		PickedQuestType = EActiveCurseQuest::ClearMaps;
		PickedTargetCount = CurseData->CurseMap;
		QuestMessage = FString::Printf(TEXT("맵을 %d개 클리어하세요!"), PickedTargetCount);
	}
	
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
	}
	
	HideSelectUI();
	
	UGameplayStatics::PlaySound2D(GetWorld(), SelectSound);
	NiagaraComp->Activate();
}

void AInteractCurse::ShowSelectUI()
{
	Super::ShowSelectUI();
	
	if (!SelectUIClass) return;
	
	if (!SelectUIInstance)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		SelectUIInstance = CreateWidget<USelectUI>(PC, SelectUIClass);
		if (SelectUIInstance)
			SelectUIInstance->ClearFlags(RF_Transactional);
	}

	if (!SelectUIInstance) return;

	SelectUIInstance->ClearButtons();
	if (!SelectUIInstance->IsInViewport())
	{
		SelectUIInstance->AddToViewport();
		UGameplayStatics::PlaySound2D(GetWorld(), InteractSound);
	}
}

void AInteractCurse::HideSelectUI()
{
	Super::HideSelectUI();
	
	if (SelectUIInstance->IsInViewport())
		SelectUIInstance->RemoveFromParent();
}
