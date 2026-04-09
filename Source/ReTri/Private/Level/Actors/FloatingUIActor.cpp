// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/FloatingUIActor.h"

#include "Components/WidgetComponent.h"
#include "Level/UI/FloatingUI.h"
#include "ReTri/ReTri.h"

// Sets default values
AFloatingUIActor::AFloatingUIActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(RootComponent);
	
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}

// 데미지 입었을 때 ex) ShowScaleUI(TEXT("20"), FLinearColor(r, g, b, a))
void AFloatingUIActor::ShowScaleUI(FText Text, FLinearColor Color)
{
	if (UFloatingUI* FloatingUI = Cast<UFloatingUI>(WidgetComp->GetUserWidgetObject()))
	{
		FloatingUI->PlayScale(Text, Color);
	}
	
	SetLifeSpan(1.0f);
}

void AFloatingUIActor::ShowFloatingUI(FText Text, FLinearColor Color)
{
	if (UFloatingUI* FloatingUI = Cast<UFloatingUI>(WidgetComp->GetUserWidgetObject()))
	{
		FloatingUI->PlayFloating(Text, Color);
	}

	SetLifeSpan(1.0f);
}

void AFloatingUIActor::ShowFloatingLongUI(FText Text, FLinearColor Color)
{
	if (UFloatingUI* FloatingUI = Cast<UFloatingUI>(WidgetComp->GetUserWidgetObject()))
	{
		FloatingUI->PlayFloatingLong(Text, Color);
	}

	SetLifeSpan(1.5f);
}
