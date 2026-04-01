// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractCurse.h"

#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"
#include "ReTri/ReTri.h"
#include "ReTriGameInstance.h"
#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"


void AInteractCurse::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Curse;
}

void AInteractCurse::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	UE_LOG(jiwon, Warning, TEXT("3가지 선택지 UI 띄우기 -> 저주를 받고 스텟 레벨업"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	// todo: 3가지 선택지 UI 띄우기
	if (!CurseDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Curse Data Table 할당 되지 않음"));
		return;
	}
	
	// Curse Data 가져오기
	CurseDatas.Empty();
	CurseDataTable->GetAllRows<FCurseData>(TEXT("Curse::Interact"), CurseDatas);
	
	ShowSelectUI();

	for (int i = 0; i < CurseDatas.Num(); i++)
	{
		USelectButtonUI* Button = SelectUIInstance->AddButton(CurseDatas[i]->CurseName, CurseDatas[i]->CurseInfo, i, CurseDatas[i]->CurseColor);
		Button->OnSelectClicked.AddDynamic(this, &AInteractCurse::OnCurseSelected);
	}
}

void AInteractCurse::OnCurseSelected(int32 Index)
{
	//todo 저주를 받고 스텟 레벨업
	FCurseData* CurseData = CurseDatas[Index];
	JIWONLOG("선택된 저주: %s", *CurseData->CurseName);
	
	int32 RandomCurse = FMath::RandRange(0, 1);
	if (RandomCurse == 0)
	{
		SCREENLOG("몬스터를 %d마리 처치하세요!", CurseData->CurseMonster);
	}
	else
	{
		SCREENLOG("맵을 %d구역 클리어하세요!", CurseData->CurseMap);
	}
	auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	CurseRewardDataTable->GetAllRows<FCurseRewardData>(TEXT("Curse::Select"), CurseRewardDatas);
	int32 RandomReward = FMath::RandRange(0, CurseRewardDatas.Num() - 1);
	
	int32 RewardVal = CurseRewardDatas[RandomReward]->RewardLevels[static_cast<int32>(CurseData->CurseType)];
	if (!GI->StatComp) return;
	switch (CurseRewardDatas[RandomReward]->RewardType)
	{
	case ERewardType::RewardGold:
		// GI->GameData->UpdateGold(+RewardVal);
		GI->StatComp->ApplyStatModifier(EStatTypes::Gold, RewardVal);
		break;
	case ERewardType::RewardDreamPowder:
		// GI->GameData->UpdateDreamPowder(+RewardVal);
		GI->StatComp->ApplyStatModifier(EStatTypes::DreamDust, RewardVal);
		break;
	case ERewardType::RewardMaxHP:
		// GI->GameData->UpdateMaxHP(+RewardVal);
		GI->StatComp->ApplyStatModifier(EStatTypes::MaxHP, RewardVal);
		break;
	case ERewardType::RewardAttackDamage:
		// GI->GameData->UpdateAttackDamage(+RewardVal);
		GI->StatComp->ApplyStatModifier(EStatTypes::AttackPower, RewardVal);
		break;
	case ERewardType::RewardMemoryHaste:
		// GI->GameData->UpdateCoolTime(+RewardVal);
		GI->StatComp->ApplyStatModifier(EStatTypes::MemoryAcceleration, RewardVal);
		break;
	}
	
	//todo Info 띄우기 	CurseRewardDatas[RandomReward]->Info;
	SCREENLOG("%s", *CurseRewardDatas[RandomReward]->Info);
	JIWONLOG("%s", *CurseRewardDatas[RandomReward]->Info);
	
	HideSelectUI();
	bIsUsed = true;
	// todo InteractableData->IsUsed = true;
}
