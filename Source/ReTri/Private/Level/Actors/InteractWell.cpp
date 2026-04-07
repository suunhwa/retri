// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractWell.h"

#include "ReTri/ReTri.h"


void AInteractWell::BeginPlay()
{
	Super::BeginPlay();
	
	// InteractableType = EInteractableType::Well;
}

void AInteractWell::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	// todo: 꿈가루로 스탯 업그레이드
	UE_LOG(jiwon, Warning, TEXT("꿈가루로 스탯 업그레이드 UI"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	if (!WellDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Well Data Table 할당 되지 않음"));
		return;
	}
	
	// Chaos Data 가져오기
	TArray<FWellRewardData*> AllWell;
	WellDataTable->GetAllRows<FWellRewardData>(TEXT("Chaos::Interact"), AllWell);
	PickedWellReward.Empty();
	
	// 랜덤으로 섞기
	for (int i = AllWell.Num()-1; i > 0; i--)
	{
		int32 R = FMath::RandRange(0, i);
		AllWell.Swap(i, R);
	}

	// UI띄우기
	ShowSelectUI();
	if (!SelectUIInstance) return;
	
	for (int i = 0; i < 3; i++)
	{
		PickedWellReward.Add(AllWell[i]);

		// FStringFormatOrderedArguments Args;
		// Args.Add(FMath::TruncToInt(AllWell[i]->ChaosValues[AllWell[i]->ChaosLevel]));
		// FString Info = FString::Format(*AllWell[i]->ChaosInfo, Args);
		// //USelectButtonUI* Button = SelectUIInstance->AddButton(AllChaos[i]->ChaosName, Info, i);
		// if (Button)
		// {
		// 	Button->OnSelectClicked.AddDynamic(this, &AInteractChaos::OnChaosSelected);
		// }
	}
}

void AInteractWell::OnChaosSelected(int32 Index)
{
}
