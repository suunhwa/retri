// Fill out your copyright notice in the Description page of Project Settings.


#include "ReTriGameInstance.h"

#include "MapSubSystem.h"
#include "ReTriGameData.h"

void UReTriGameInstance::Init()
{
	Super::Init();
	
	if (GameDataClass)
		GameData = NewObject<UReTriGameData>(this, GameDataClass);
	if (nullptr == GameData)  
		UE_LOG(LogTemp, Error, TEXT("[UReTriGameInstance] GameDataClass 할당안됨"));

	UMapSubSystem* MapSub = GetSubsystem<UMapSubSystem>();
	if (MapSub)
	{
		MapSub->InteractionData = this->InteractionData;
		MapSub->MapUIData = this->MapUIData;
		MapSub->SkillDataTable = this->SkillDataTable;
		
		MapSub->GoldCoinPotClass = this->GoldCoinPotClass;
		MapSub->DreamPowderClass = this->DreamPowderClass;
		
		MapSub->ProceduralGenerateMap();
	}
}
