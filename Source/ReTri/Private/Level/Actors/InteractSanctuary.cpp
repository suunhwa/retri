// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractSanctuary.h"

#include "ReTriGameData.h"
#include "ReTriGameInstance.h"
#include "Player/Components/StatComponent.h"


void AInteractSanctuary::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Sanctuary;
}

void AInteractSanctuary::Interact_Implementation()
{
	// Super::Interact_Implementation();
	if (!MyPlayer) return;
	
	// todo 플레이어 골드 뺏고 
	// todo 플레이어 회복
	
	auto GI = Cast<UReTriGameInstance>(GetGameInstance());
	if (!GI || !GI->StatComp) return;
	if (!GI->StatComp->SpendGold(static_cast<int32>(Cost))) return;
	GI->GameData->UpdateHP(+HealHP);
	
	// UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	Super::Interact_Implementation();
}
