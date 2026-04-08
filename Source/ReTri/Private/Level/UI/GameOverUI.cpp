// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/GameOverUI.h"

#include "ReTriGameData.h"
#include "ReTriGameInstance.h"
#include "MapSubSystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Level/UI/MiniStatUI.h"
#include "Level/UI/PlayDataUI.h"
#include "Player/Components/HealthComponent.h"
#include "Player/Components/StatComponent.h"

void UGameOverUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	RetryButton->OnClicked.AddDynamic(this, &UGameOverUI::Retry);
	QuitButton->OnClicked.AddDynamic(this, &UGameOverUI::Quit);
}


void UGameOverUI::SetUIData()
{	
	auto* GI = Cast<UReTriGameInstance>(GetGameInstance());
	if (!GI) return;
	
	// Stat
	FString T = FString::Printf(TEXT("%0.f"), GI->HealthComp->GetMaxHP());
	MiniStatUI->MaxHpText->SetText(FText::FromString(T));
	T = FString::Printf(TEXT("%0.f"), GI->StatComp->GetAttackPower());
	MiniStatUI->AttackDamageText->SetText(FText::FromString(T));
	T = FString::Printf(TEXT("%0.f"), GI->StatComp->GetSpellPower());
	MiniStatUI->AbilityPowerText->SetText(FText::FromString(T));
	T = FString::Printf(TEXT("%0.f"), GI->StatComp->GetAttackSpeed());
	MiniStatUI->AttackSpeedText->SetText(FText::FromString(T));
	T = FString::Printf(TEXT("%0.f"), GI->StatComp->GetMemoryAcceleration());
	MiniStatUI->CoolTimeText->SetText(FText::FromString(T));
	T = FString::Printf(TEXT("%0.f"), GI->StatComp->GetCritRate());
	MiniStatUI->CriticalText->SetText(FText::FromString(T));
	T = FString::Printf(TEXT("%0.f"), GI->StatComp->GetBurnDamageBonus());
	MiniStatUI->FireText->SetText(FText::FromString(T));
	
	// todo 선화에게 물어보기
	// Skill 
	//SaveSkillSlot->SkillSlotUI_0->InitSlot()
	
	// ScoreText
	ScoreText->SetText(FText::AsNumber(GI->StatComp->GetCurrentExp()));
	
	// PlayDataUI
	FPlayerPlayData PlayData = GI->PlayerPlayData;
	PlayDataUI_0->SetTexts(TEXT("사망"), 1);
	PlayDataUI_1->SetTexts(TEXT("방문한 장소"), PlayData.VisitLevel);
	PlayDataUI_2->SetTexts(TEXT("처치한 괴물"), PlayData.KillEnemy);
	PlayDataUI_3->SetTexts(TEXT("처치한 보스"), PlayData.KillBoss);
	PlayDataUI_4->SetTexts(TEXT("적들에게 가한 피해량"), PlayData.GiveDamage);
	PlayDataUI_5->SetTexts(TEXT("받은 피해량"), PlayData.GetDamage);
	PlayDataUI_6->SetTexts(TEXT("자가 회복량"), PlayData.Heal);
	PlayDataUI_7->SetTexts(TEXT("획득한 골드"), PlayData.Gold);
	PlayDataUI_8->SetTexts(TEXT("획득한 별가루"), PlayData.DreamDust);
}

void UGameOverUI::Retry()
{
	if (auto* GI = Cast<UReTriGameInstance>(GetGameInstance()))
	{
		GI->bHasSavedStats = false;
		GI->SavedSkillSlots.Empty();
		GI->PlayerPlayData = FPlayerPlayData();
		
		if (auto* MapSub = GI->GetSubsystem<UMapSubSystem>())
		{
			MapSub->ProceduralGenerateMap();
		}
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("Lv_Main"));
}

void UGameOverUI::Quit()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PlayerController, EQuitPreference::Quit, true);
}
