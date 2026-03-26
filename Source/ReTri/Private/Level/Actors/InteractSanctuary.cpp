// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractSanctuary.h"

#include "ReTriGameData.h"
#include "Level/Actors/NZW_TestPlayer.h"

#include "Components/CapsuleComponent.h"


void AInteractSanctuary::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Sanctuary;
}

void AInteractSanctuary::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	if (!MyPlayer) return;
	
	// todo 플레이어 골드 뺏고 
	// todo 플레이어 회복
	
	// if (MyPlayer->GD->GetGold() <= Cost) return;
	// MyPlayer->GD->UpdateGold(-Cost);
	// MyPlayer->GD->UpdateHP(+HealHP);
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InteractName);
}
