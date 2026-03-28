// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractCurse.h"


void AInteractCurse::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Curse;
}

void AInteractCurse::Interact_Implementation()
{
	// todo: 3가지 선택지 UI 띄우기 -> 저주를 받고 스텟 레벨업
	UE_LOG(jiwon, Warning, TEXT("3가지 선택지 UI 띄우기 -> 저주를 받고 스텟 레벨업"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	Super::Interact_Implementation();
}
