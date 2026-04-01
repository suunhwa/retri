// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/ExpBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UExpBar::OnExpChanged(int32 CurrentExp, int32 RequiredExp, int32 CurrentLevel)
{
	if (ExpBar)
	{
		// RequiredExp == -1이면 맥스 레벨 → 바 꽉 채움
		const float Percent = (RequiredExp > 0) ? FMath::Clamp(static_cast<float>(CurrentExp) / static_cast<float>(RequiredExp), 0.f, 1.f) : 1.f;
		ExpBar->SetPercent(Percent);
	}
	
	if (LevelTxt)
	{
		LevelTxt->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), CurrentLevel)));
	}
}
