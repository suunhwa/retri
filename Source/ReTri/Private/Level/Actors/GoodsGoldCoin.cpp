// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsGoldCoin.h"

#include "ReTriGameData.h"
#include "Level/Actors/NZW_TestPlayer.h"


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
	
	// 1. 부딪힌 녀석이 플레이어가 맞는지 제일 먼저 확인
	if (OtherActor == TargetPlayer)
	{
		ANZW_TestPlayer* Player = Cast<ANZW_TestPlayer>(OtherActor);
		if (Player && Player->GD) // GD 컴포넌트가 무사히 달려있는지 안전벨트
		{
			// 헤더파일에 있는 Gold 변수가 0이 아닌지 꼭 확인해 보세요!
			Player->GD->UpdateGold(Gold); 
			
			// 2. 골드를 무사히 넘겨줬을 때만 금화가 화면에서 사라지게(파괴) 합니다.
			Destroy();
		}
	}
	// 플레이어가 아닌 것(바닥, 벽 등)과 부딪히면 아무 일도 일어나지 않고 무시합니다.
}

