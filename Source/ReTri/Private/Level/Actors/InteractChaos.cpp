// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractChaos.h"

#include "ReTriGameInstance.h"
#include "Level/UI/SelectButtonUI.h"
#include "Level/UI/SelectUI.h"
#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"
#include "Level/Actors/FloatingUIActor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


AInteractChaos::AInteractChaos()
{
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagara(TEXT("/Game/Free_Magic/VFX_Niagara/NS_Chaos.NS_Chaos"));
	if (TempNiagara.Succeeded()) NiagaraComp->SetAsset(TempNiagara.Object);
	NiagaraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	NiagaraComp->bAutoActivate = false;
}

void AInteractChaos::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractChaos::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

void AInteractChaos::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	SetIsUsed(true);
	
	if (!ChaosDataTable) return;
	
	TArray<FChaosData*> AllChaos;
	ChaosDataTable->GetAllRows<FChaosData>(TEXT("Chaos::Interact"), AllChaos);
	PickedChaoses.Empty();
	
	for (int i = AllChaos.Num()-1; i > 0; i--)
	{
		int32 R = FMath::RandRange(0, i);
		AllChaos.Swap(i, R);
	}

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
	
	UGameplayStatics::PlaySound2D(GetWorld(), SelectSound);
	NiagaraComp->Activate();
}

void AInteractChaos::ShowSelectUI()
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

void AInteractChaos::HideSelectUI()
{
	Super::HideSelectUI();
	
	if (SelectUIInstance->IsInViewport())
		SelectUIInstance->RemoveFromParent();
}
