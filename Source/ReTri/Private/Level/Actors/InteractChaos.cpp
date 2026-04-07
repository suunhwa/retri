// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractChaos.h"

#include "MapSubSystem.h"
#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"
#include "ReTriGameInstance.h"
#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"
#include "Level/Actors/FloatingUIActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


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
	
	FString FloatingText;
	FLinearColor FloatingColor = FLinearColor::White;
	
	switch (ChaosData->ChaosType)
	{
	case EChaosType::Chaos_Health:
		GI->StatComp->ApplyStatModifier(EStatTypes::MaxHP, Val);
		GI->HealthComp->Heal(GI->StatComp->GetMaxHP());
		FloatingText = FString::Printf(TEXT("최대 체력 +%d"), FMath::TruncToInt(Val));
		FloatingColor = FLinearColor(0.1f, 1.0f, 0.1f, 1.f); // Green
		break;
	case EChaosType::Chaos_AttackDamage:
		GI->StatComp->ApplyStatModifier(EStatTypes::AttackPower, Val);
		FloatingText = FString::Printf(TEXT("공격력 +%d"), FMath::TruncToInt(Val));
		FloatingColor = FLinearColor(1.0f, 0.1f, 0.1f, 1.f); // Red
		break;
	case EChaosType::Chaos_AbilityPower:
		GI->StatComp->ApplyStatModifier(EStatTypes::SpellPower, Val);
		FloatingText = FString::Printf(TEXT("주문력 +%d"), FMath::TruncToInt(Val));
		FloatingColor = FLinearColor(0.1f, 0.5f, 1.0f, 1.f); // Cyan
		break;
	case EChaosType::Chaos_AttackSpeed:
		{
			float OriginalVal = ChaosData->ChaosValues[ChaosData->ChaosLevel - 1];
			Val = GI->StatComp->GetAttackSpeed() * (OriginalVal - 1.0f);
			GI->StatComp->ApplyStatModifier(EStatTypes::AttackSpeed, Val);
			FloatingText = FString::Printf(TEXT("공격 속도 +%d%%"), FMath::RoundToInt((OriginalVal - 1.0f) * 100.f));
			FloatingColor = FLinearColor(1.0f, 0.8f, 0.0f, 1.f); // Yellow
		}
		break;
	case EChaosType::Chaos_MemoryHaste:
		GI->StatComp->ApplyStatModifier(EStatTypes::MemoryAcceleration, Val);
		FloatingText = FString::Printf(TEXT("기억 가속 +%d"), FMath::TruncToInt(Val));
		FloatingColor = FLinearColor(0.8f, 0.2f, 0.8f, 1.f); // Purple
		break;
	}

	if (GI->FloatingUIActorClass && !FloatingText.IsEmpty())
	{
		if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			AFloatingUIActor* FloatingUIActor = GetWorld()->SpawnActor<AFloatingUIActor>(
				GI->FloatingUIActorClass, Player->GetActorLocation(), FRotator::ZeroRotator);
			FloatingUIActor->ShowFloatingUI(FText::FromString(FloatingText), FloatingColor);
		}
	}
	
	GI->DebugStat();
	
	HideSelectUI();
}
