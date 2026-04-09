// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/LootGoldCoinPot.h"

#include "Kismet/GameplayStatics.h"
#include "Level/Actors/GoodsGoldCoin.h"

ALootGoldCoinPot::ALootGoldCoinPot()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Stylized_PBR_Nature/Rocks/Assets/SM_S_Rock_04.SM_S_Rock_04'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void ALootGoldCoinPot::Break()
{
	// Destory 함수가 부모한테 있으니까
	Super::Break();

	UGameplayStatics::PlaySound2D(GetWorld(), BreakSound1);
	
	// Spawn Goods!
	GetWorld()->SpawnActor<AGoodsGoldCoin>(GoodsClass, GetActorLocation(), GetActorRotation());
	
	// Effect Play
	// UE_LOG(jiwon, Warning, TEXT("ALootGoldCoinPot"));
}
