// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractRemnants.h"


void AInteractRemnants::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Remnants;
}

void AInteractRemnants::Interact_Implementation()
{
	// todo: 스킬을 랜덤으로 스폰
	UE_LOG(jiwon, Warning, TEXT("스킬을 랜덤으로 스폰"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	Super::Interact_Implementation();
}
