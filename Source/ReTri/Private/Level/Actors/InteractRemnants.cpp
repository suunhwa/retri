// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractRemnants.h"

#include "MapSubSystem.h"


void AInteractRemnants::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Remnants;
}

void AInteractRemnants::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	// todo: 스킬을 랜덤으로 스폰
	UE_LOG(jiwon, Warning, TEXT("스킬을 랜덤으로 스폰"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	
	FName KeyName(*InteractName);
	bool* FoundValue = GetGameInstance()->GetSubsystem<UMapSubSystem>()->GetCurMapData().SpawnInteractableRowNames.Find(KeyName);
	if (FoundValue) *FoundValue = true; 	
}
