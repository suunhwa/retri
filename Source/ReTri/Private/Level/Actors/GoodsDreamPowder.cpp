// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsDreamPowder.h"

#include "ReTriGameData.h"
#include "ReTriGameInstance.h"

AGoodsDreamPowder::AGoodsDreamPowder()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/gold-star/source/SM_Dream.SM_Dream'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);  
}

void AGoodsDreamPowder::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoodsDreamPowder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoodsDreamPowder::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor == TargetPlayer)
	{
		auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
		GI->GameData->PlayerStats.DreamPowder += GI->GameData->GetRandomDreamPowder();
		//todo 선화가 만든 Stat으로 바꿔야함 GI->CurPlayerStat.DreamPowder += GI->GameData->GetRandomDreamPowder();
		
		GI->GameData->DebugStat();
		Destroy();
	}
}
