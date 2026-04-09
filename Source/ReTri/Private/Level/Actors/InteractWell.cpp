// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractWell.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "ReTriGameInstance.h"
#include "Level/Actors/FloatingUIActor.h"
#include "Level/UI/WellSelectButtonUI.h"
#include "Level/UI/WellSelectUI.h"
#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ReTri/ReTri.h"


AInteractWell::AInteractWell()
{
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagara(TEXT("/Game/Free_Magic/VFX_Niagara/NS_Sanctuary.NS_Sanctuary"));
	if (TempNiagara.Succeeded()) NiagaraComp->SetAsset(TempNiagara.Object);
	NiagaraComp->SetRelativeRotation(FRotator(-90.0f, -0.0f, 0.0f));
	NiagaraComp->bAutoActivate = false;
}

void AInteractWell::BeginPlay()
{
	Super::BeginPlay();
	
	
	// InteractableType = EInteractableType::Well;
}

void AInteractWell::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// GC에서 해결 되지 않고 플레이를 끈 경우 
	if (WellSelectUIInstance)
	{
		WellSelectUIInstance->ClearButtons();
		if (WellSelectUIInstance->IsInViewport())
		{
			WellSelectUIInstance->RemoveFromParent();
		}
		WellSelectUIInstance = nullptr;
	}
	
	Super::EndPlay(EndPlayReason);
}

void AInteractWell::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	// UE_LOG(jiwon, Warning, TEXT("꿈가루로 스탯 업그레이드 UI"));
	// UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	if (!WellDataTable)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Well Data Table 할당 되지 않음"));
		return;
	}
	
	// Chaos Data 가져오기
	TArray<FWellRewardData*> AllWell;
	WellDataTable->GetAllRows<FWellRewardData>(TEXT("Well::Interact"), AllWell);
	PickedWellReward.Empty();
	
	// 랜덤으로 섞기
	for (int i = AllWell.Num()-1; i > 0; i--)
	{
		int32 R = FMath::RandRange(0, i);
		AllWell.Swap(i, R);
	}

	// UI띄우기
	ShowSelectUI();
	if (!WellSelectUIInstance) return;
	
	for (int i = 0; i < 3; i++)
	{
		PickedWellReward.Add(AllWell[i]);
		
		FStringFormatOrderedArguments Args;
		Args.Add(FMath::TruncToInt(AllWell[i]->RewardAmount));
		FString WellDescription = FString::Format(*AllWell[i]->WellRewardNameKR, Args); 
		FString Cost = FString::Printf(TEXT("%d"), AllWell[i]->DreamDustCost); 
		UWellSelectButtonUI* Button = WellSelectUIInstance->AddButton(*WellDescription, *Cost, i);
		if (Button)
		{
			Button->OnWellSelectClicked.AddDynamic(this, &AInteractWell::OnWellSelected);
		}
	}
}

void AInteractWell::OnWellSelected(int32 Index)
{
	FWellRewardData* WellRewardData = PickedWellReward[Index];
	// JIWONLOG("선택된 우물: %s", *WellRewardData->WellRewardNameKR);
	
	auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI || !GI->StatComp) return;
	
	// 꿈가루 부족할 경우 
	FString FloatingText = FString::Printf(TEXT("꿈가루 부족!"));
	FLinearColor FloatingColor = FLinearColor::Red;
	
	if (!GI->StatComp->SpendDreamDust(WellRewardData->DreamDustCost))
	{
		if (GI->FloatingUIActorClass && !FloatingText.IsEmpty())
		{
			if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				AFloatingUIActor* FloatingUIActor = GetWorld()->SpawnActor<AFloatingUIActor>(
					GI->FloatingUIActorClass, Player->GetActorLocation(), FRotator::ZeroRotator);
				FloatingUIActor->ShowFloatingUI(FText::FromString(FloatingText), FloatingColor);
				
				UGameplayStatics::PlaySound2D(GetWorld(), FailedSound);
			}
		}
		
		HideSelectUI();
		return;
	}
	
	FStringFormatOrderedArguments Args;
	float Val = WellRewardData->RewardAmount;
	switch (WellRewardData->WellRewardType)
	{
	case EWellRewardType::RewardMaxHP:
		GI->StatComp->ApplyStatModifier(EStatTypes::MaxHP, Val);
		GI->HealthComp->Heal(GI->StatComp->GetMaxHP());
		Args.Add(FMath::TruncToInt(Val));
		FloatingText = FString::Format(*WellRewardData->WellRewardNameKR, Args); 
		FloatingColor = FLinearColor(0.1f, 1.0f, 0.1f, 1.f); // Green
		break;
	case EWellRewardType::RewardAttackPower:
		GI->StatComp->ApplyStatModifier(EStatTypes::AttackPower, Val);
		Args.Add(FMath::TruncToInt(Val));
		FloatingText = FString::Format(*WellRewardData->WellRewardNameKR, Args); 
		FloatingColor = FLinearColor(1.0f, 0.1f, 0.1f, 1.f); // Red
		break;
	case EWellRewardType::RewardSpellPower:
		GI->StatComp->ApplyStatModifier(EStatTypes::SpellPower, Val);
		Args.Add(FMath::TruncToInt(Val));
		FloatingText = FString::Format(*WellRewardData->WellRewardNameKR, Args); 
		FloatingColor = FLinearColor(0.1f, 0.5f, 1.0f, 1.f); // Cyan
		break;
	case EWellRewardType::RewardProjectileSpeed:
		GI->StatComp->ApplyStatModifier(EStatTypes::AttackSpeed, Val);
		Args.Add(FMath::TruncToInt(Val));
		FloatingText = FString::Format(*WellRewardData->WellRewardNameKR, Args); 
		FloatingColor = FLinearColor(1.0f, 0.5f, 0.5f, 1.f); // 
		break;
	case EWellRewardType::RewardAttackSpeed:
		GI->StatComp->ApplyStatModifier(EStatTypes::AttackSpeed,  Val / 100.f);
		Args.Add(FMath::TruncToInt(Val));
		FloatingText = FString::Format(*WellRewardData->WellRewardNameKR, Args); 
		FloatingColor = FLinearColor(1.0f, 0.8f, 0.0f, 1.f); // Yellow
		break;
	case EWellRewardType::RewardMoveSpeed:
		GI->StatComp->ApplyStatModifier(EStatTypes::SpellPower, Val);
		Args.Add(FMath::TruncToInt(Val));
		FloatingText = FString::Format(*WellRewardData->WellRewardNameKR, Args); 
		FloatingColor = FLinearColor(0.1f, 1.0f, 0.7f, 1.f); // 
		break;
	case EWellRewardType::RewardMemoryAcceleration:
		GI->StatComp->ApplyStatModifier(EStatTypes::SpellPower,  Val / 100.f);
		Args.Add(FMath::TruncToInt(Val));
		FloatingText = FString::Format(*WellRewardData->WellRewardNameKR, Args); 
		FloatingColor = FLinearColor(0.1f, 0.7f, 1.0f, 1.f); // 
		break;
	}
	
	SetIsUsed(true);
	
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

void AInteractWell::ShowSelectUI()
{
	Super::ShowSelectUI();
	
	if (!SelectUIClass)
	{
		/// UE_LOG(LogTemp, Warning, TEXT("Select UI Class 할당 안됨"));
		return;
	}
	
	// 최소한 한 번만 생성, 이후에는 인스턴스 재사용
	if (!WellSelectUIInstance)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		WellSelectUIInstance = CreateWidget<UWellSelectUI>(PC, SelectUIClass);
		if (WellSelectUIInstance)
		{
			// PIE Undo Buffer Leaks 방지
			// 위 코드는 위젯을 생성하자마자 
			// "이 위젯은 실행 취소 기록에 남기지 마! 넌 그냥 게임 플레이 도중에 쓰고 버릴 일회용이야!" 
			// 라고 플래그(RF_Transactional)를 강제로 지워버리는 역할
			WellSelectUIInstance->ClearFlags(RF_Transactional);
		}
	}

	if (!WellSelectUIInstance) return;

	// 이전 버튼들 정리 
	WellSelectUIInstance->ClearButtons();
	if (!WellSelectUIInstance->IsInViewport())
	{
		WellSelectUIInstance->AddToViewport();
		UGameplayStatics::PlaySound2D(GetWorld(), InteractSound);
	}
}

void AInteractWell::HideSelectUI()
{
	Super::HideSelectUI();
	
	if (WellSelectUIInstance->IsInViewport())
	{
		WellSelectUIInstance->RemoveFromParent();
	}
}