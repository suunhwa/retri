// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractChaos.h"


void AInteractChaos::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Chaos;
}

void AInteractChaos::Interact_Implementation()
{
	// todo: 스탯 선택하는 UI 띄우고 선택하면 해당 스탯 UP!!
	UE_LOG(jiwon, Warning, TEXT("스탯 선택하는 UI 띄우고 선택하면 해당 스탯 UP!!"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	Super::Interact_Implementation();
}
