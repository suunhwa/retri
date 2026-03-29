// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/HPBar.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UHPBar::OnHPChanged(float CurrentHP, float MaxHP)
{
	if (!HPBar) return;

	HPBar->SetPercent(MaxHP > 0.f ? CurrentHP / MaxHP : 0.f);
}
