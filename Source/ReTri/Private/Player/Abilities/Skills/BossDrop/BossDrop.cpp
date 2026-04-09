// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/Skills/BossDrop/BossDrop.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Player/Components/AbilityComponent.h"

UBossDrop::UBossDrop()
{
	DataTableRowName = TEXT("boss_drop");
}

bool UBossDrop::CanActivate(ACharacter* Owner)
{
	if (!Owner) return false;

	return Super::CanActivate(Owner);
}

void UBossDrop::Activate(ACharacter* Owner)
{
	if (!Owner) return;

	// TODO: 레벨 이름 넣으면 됩니당
	// if (!NextLevelName.IsNone())
	//     UGameplayStatics::OpenLevel(Owner->GetWorld(), NextLevelName);

	Super::Activate(Owner);
}
