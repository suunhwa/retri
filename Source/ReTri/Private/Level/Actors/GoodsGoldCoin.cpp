// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsGoldCoin.h"

#include "ReTriGameData.h"
#include "ReTriGameInstance.h"
#include "Level/Actors/FloatingUIActor.h"


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
		int32 CurGold = GI->GameData->GetRandomDreamPowder();
		GI->GameData->UpdateGold(+GI->GameData->GetRandomGold());
		//todo 선화가 만든 Stat으로 바꿔야함 GI->CurPlayerStat.CurGold += GI->GameData->GetRandomGold();
		
		if (FloatingUIActorClass)
		{
			AFloatingUIActor* FloatingUI = GetWorld()->SpawnActor<AFloatingUIActor>(FloatingUIActorClass, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
			FString TempStr = FString::Printf(TEXT("골드 +%d"), CurGold);
			FloatingUI->ShowFloatingUI(FText::FromString(TempStr), FLinearColor(1.f, 0.617f, 0.f, 1.f));
		}
		
		GI->GameData->DebugStat();
		Destroy();
	}
}

