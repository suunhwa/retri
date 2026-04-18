

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

	if (MiniStatUI)
	{
		auto SetText = [](UTextBlock* Block, float Val)
		{
			if (Block) Block->SetText(FText::FromString(FString::Printf(TEXT("%0.f"), Val)));
		};
		SetText(MiniStatUI->MaxHpText,          GI->Snapshot_MaxHP);
		SetText(MiniStatUI->AttackDamageText,   GI->Snapshot_AttackPower);
		SetText(MiniStatUI->AbilityPowerText,   GI->Snapshot_SpellPower);
		SetText(MiniStatUI->AttackSpeedText,    GI->Snapshot_AttackSpeed);
		SetText(MiniStatUI->CoolTimeText,       GI->Snapshot_MemoryAcceleration);
		SetText(MiniStatUI->CriticalText,       GI->Snapshot_CritRate);
		SetText(MiniStatUI->FireText,           GI->Snapshot_BurnDamageBonus);
	}

	if (ScoreText) ScoreText->SetText(FText::AsNumber(GI->Snapshot_CurrentExp));

	FPlayerPlayData PlayData = GI->PlayerPlayData;
	if (PlayDataUI_0) PlayDataUI_0->SetTexts(TEXT("사망"), 1);
	if (PlayDataUI_1) PlayDataUI_1->SetTexts(TEXT("방문한 장소"), PlayData.VisitLevel);
	if (PlayDataUI_2) PlayDataUI_2->SetTexts(TEXT("처치한 괴물"), PlayData.KillEnemy);
	if (PlayDataUI_3) PlayDataUI_3->SetTexts(TEXT("처치한 보스"), PlayData.KillBoss);
	if (PlayDataUI_4) PlayDataUI_4->SetTexts(TEXT("적들에게 가한 피해량"), PlayData.GiveDamage);
	if (PlayDataUI_5) PlayDataUI_5->SetTexts(TEXT("받은 피해량"), PlayData.GetDamage);
	if (PlayDataUI_6) PlayDataUI_6->SetTexts(TEXT("자가 회복량"), PlayData.Heal);
	if (PlayDataUI_7) PlayDataUI_7->SetTexts(TEXT("획득한 골드"), PlayData.Gold);
	if (PlayDataUI_8) PlayDataUI_8->SetTexts(TEXT("획득한 별가루"), PlayData.DreamDust);
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
