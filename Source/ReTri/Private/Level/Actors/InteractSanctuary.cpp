// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractSanctuary.h"

#include "MapSubSystem.h"
#include "ReTriGameData.h"
#include "ReTriGameInstance.h"
#include "Player/Components/HealthComponent.h"
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

	if (!GI->StatComp->SpendGold(Cost))
	{
		SCREENLOG("돈없음!!");
		return;
	}
	
	GI->HealthComp->Heal(HealHP);
	
	FName KeyName = FName("Sanctuary");
	bool* FoundValue = GetGameInstance()->GetSubsystem<UMapSubSystem>()->GetCurMapData().SpawnInteractableRowNames.Find(KeyName);
	if (FoundValue) *FoundValue = true; 
	SetIsUsed(true);
	
	// UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	Super::Interact_Implementation();
}
