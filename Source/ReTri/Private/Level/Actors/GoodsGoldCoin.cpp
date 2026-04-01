// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsGoldCoin.h"

#include "ReTriGameData.h"
#include "ReTriGameInstance.h"
#include "Player/Components/StatComponent.h"


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
		/*GI->GameData->UpdateGold(+GI->GameData->GetRandomGold());
		//todo 선화가 만든 Stat으로 바꿔야함 GI->CurPlayerStat.CurGold += GI->GameData->GetRandomGold();*/
		if (GI && GI->StatComp)
		{
			const int32 Amount = GI->GameData->GetRandomGold();
			GI->StatComp->ApplyStatModifier(EStatTypes::Gold, Amount);
		}
		
		GI->GameData->DebugStat();
		Destroy();
	}
}

