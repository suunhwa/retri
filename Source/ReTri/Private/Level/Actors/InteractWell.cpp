// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractWell.h"


void AInteractWell::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Well;
}

void AInteractWell::Interact_Implementation()
{
	// todo: 스킬 Level Up! UI
	UE_LOG(jiwon, Warning, TEXT("스킬 Level Up! UI"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	Super::Interact_Implementation();
}
