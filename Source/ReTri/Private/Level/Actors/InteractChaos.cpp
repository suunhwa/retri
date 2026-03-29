// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractChaos.h"

#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"


void AInteractChaos::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Chaos;
}

void AInteractChaos::Interact_Implementation()
{
	Super::Interact_Implementation();
	UE_LOG(jiwon, Warning, TEXT("스탯 선택하는 UI 띄우고 선택하면 해당 스탯 UP!!"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	// todo: 스탯 선택하는 UI 띄우고 선택하면 해당 스탯 UP!!
	if (!ChaosDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Curse Data Table 할당 되지 않음"));
		return;
	}
	
	// Curse Data 가져오기
	TArray<FChaosData*> AllChaos;
	ChaosDataTable->GetAllRows<FChaosData>(TEXT("Chaos::Interact"), AllChaos);
	PickedChaoses.Empty();
	
	// 랜덤으로 섞기
	for (int i = AllChaos.Num()-1; i > 0; i--)
	{
		int32 R = FMath::RandRange(0, i);
		AllChaos.Swap(i, R);
	}
	
	// UI띄우기
	ShowSelectUI();

	for (int i = 0; i < 3; i++)
	{
		PickedChaoses.Add(AllChaos[i]);
		FStringFormatOrderedArguments Args;
		Args.Add(AllChaos[i]->ChaosValues[AllChaos[i]->ChaosLevel]); // 배열의 값을 컨테이너에 넣기
		FString Info = FString::Format(*AllChaos[i]->ChaosInfo, Args);
		USelectButtonUI* Button = SelectUIInstance->AddButton(AllChaos[i]->ChaosName, Info, i);
		Button->OnSelectClicked.AddDynamic(this, &AInteractChaos::OnChaosSelected);
	}
}

void AInteractChaos::OnChaosSelected(int32 Index)
{
	FChaosData* ChaosData = PickedChaoses[Index];
	JIWONLOG("선택된 저주: %s", *ChaosData->ChaosName);
	
	// todo: MyPlayer에게 Chaos 효과 적용

	HideSelectUI();
	bIsUsed = true;
	// todo InteractableData->IsUsed = true;
}
