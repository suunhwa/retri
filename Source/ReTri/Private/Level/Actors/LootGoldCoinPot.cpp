// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/LootGoldCoinPot.h"

#include "Level/Actors/GoodsGoldCoin.h"

ALootGoldCoinPot::ALootGoldCoinPot()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/GoldPot_Test.GoldPot_Test'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void ALootGoldCoinPot::Break()
{
	// Destory 함수가 부모한테 있으니까
	Super::Break();
	
	// Spawn Goods!
	GetWorld()->SpawnActor<AGoodsGoldCoin>(GoodsClass, GetActorLocation(), GetActorRotation());
	
	// Effect Play
	UE_LOG(LogTemp, Warning, TEXT("ALootGoldCoinPot"));
}
