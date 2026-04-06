// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractChaos.h"

#include "MapSubSystem.h"
#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"
#include "ReTriGameInstance.h"
#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"


void AInteractChaos::BeginPlay()
{
	Super::BeginPlay();
	
	//InteractableType = EInteractableType::Chaos;
}

void AInteractChaos::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	UE_LOG(jiwon, Warning, TEXT("스탯 선택하는 UI 띄우고 선택하면 해당 스탯 UP!!"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	// FName KeyName = FName("Chaos");
	// bool* FoundValue = GetGameInstance()->GetSubsystem<UMapSubSystem>()->GetCurMapData().SpawnInteractableRowNames.Find(KeyName);
	// if (FoundValue) *FoundValue = true;
	
	SetIsUsed(true);
	
	if (!ChaosDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Chaos Data Table 할당 되지 않음"));
		return;
	}
	
	// Chaos Data 가져오기
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
	if (!SelectUIInstance) return;
	
	for (int i = 0; i < 3; i++)
	{
		PickedChaoses.Add(AllChaos[i]);

		FStringFormatOrderedArguments Args;
		Args.Add(FMath::TruncToInt(AllChaos[i]->ChaosValues[AllChaos[i]->ChaosLevel]));
		FString Info = FString::Format(*AllChaos[i]->ChaosInfo, Args);
		USelectButtonUI* Button = SelectUIInstance->AddButton(AllChaos[i]->ChaosName, Info, i);
		if (Button)
		{
			Button->OnSelectClicked.AddDynamic(this, &AInteractChaos::OnChaosSelected);
		}
	}
}

void AInteractChaos::OnChaosSelected(int32 Index)
{
	FChaosData* ChaosData = PickedChaoses[Index];
	JIWONLOG("선택된 혼돈: %s", *ChaosData->ChaosName);
	
	auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI || !GI->StatComp) return;
	
	float Val = ChaosData->ChaosValues[ChaosData->ChaosLevel++];
	switch (ChaosData->ChaosType)
	{
	case EChaosType::Chaos_Health:
		GI->StatComp->ApplyStatModifier(EStatTypes::MaxHP, Val);
		GI->HealthComp->Heal(GI->StatComp->GetMaxHP());
		break;
	case EChaosType::Chaos_AttackDamage:
		GI->StatComp->ApplyStatModifier(EStatTypes::AttackPower, Val);
		break;
	case EChaosType::Chaos_AbilityPower:
		GI->StatComp->ApplyStatModifier(EStatTypes::SpellPower, Val);
		break;
	case EChaosType::Chaos_AttackSpeed:
		Val = GI->StatComp->GetAttackSpeed() * (Val - 1.0f);
		GI->StatComp->ApplyStatModifier(EStatTypes::AttackSpeed, Val);
		break;
	case EChaosType::Chaos_MemoryHaste:
		GI->StatComp->ApplyStatModifier(EStatTypes::MemoryAcceleration, Val);
		break;
	}

	GI->DebugStat();
	
	HideSelectUI();
}
