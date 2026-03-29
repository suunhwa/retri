// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractCurse.h"

#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"


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
	
	HideSelectUI();
	bIsUsed = true;
	// todo InteractableData->IsUsed = true;
}
