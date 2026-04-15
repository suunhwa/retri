#include "Player/UI/PlayerHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player/UI/SkillBarUI.h"

void UPlayerHUD::InitSkillBar(UAbilityComponent* InAbilityComp)
{
	if (SkillBar && InAbilityComp)
		SkillBar->InitSkillBar(InAbilityComp);
}

void UPlayerHUD::OnHPChanged(float CurrentHP, float MaxHP)
{
	if (HPBar)
		HPBar->SetPercent(MaxHP > 0.f ? CurrentHP / MaxHP : 0.f);

	if (HPText)
		HPText->SetText(
			FText::FromString(
				FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(CurrentHP), FMath::RoundToInt(MaxHP))));
}

void UPlayerHUD::OnStatChanged(EStatTypes StatType, float NewValue)
{
	if (StatType == EStatTypes::Gold && GoldText)
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::RoundToInt(NewValue))));

	if (StatType == EStatTypes::DreamDust && DreamPowderText)
		DreamPowderText->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::RoundToInt(NewValue))));
}

void UPlayerHUD::OnExpChanged(int32 CurrentExp, int32 RequiredExp, int32 CurrentLevel)
{
	if (ExpBar)
	{
		const float Percent = (RequiredExp > 0) ? FMath::Clamp((float)CurrentExp / (float)RequiredExp, 0.f, 1.f) : 1.f;
		ExpBar->SetPercent(Percent);
	}

	if (LevelText)
		LevelText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentLevel)));
}
