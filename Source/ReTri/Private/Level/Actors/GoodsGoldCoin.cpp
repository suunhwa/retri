// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsGoldCoin.h"

#include "ReTriGameData.h"
#include "ReTriGameInstance.h"


AGoodsGoldCoin::AGoodsGoldCoin()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/low-poly_gold_coin/StaticMeshes/SM_Gold.SM_Gold'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void AGoodsGoldCoin::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoodsGoldCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGoodsGoldCoin::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor == TargetPlayer)
	{
		auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
		GI->GameData->PlayerStats.Gold += GI->GameData->GetRandomGold();
		//todo 선화가 만든 Stat으로 바꿔야함 GI->CurPlayerStat.CurGold += GI->GameData->GetRandomGold();
		
		GI->GameData->DebugStat();
		Destroy();
	}
}

