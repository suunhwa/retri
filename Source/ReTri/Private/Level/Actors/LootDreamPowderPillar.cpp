// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/LootDreamPowderPillar.h"

#include "Level/Actors/GoodsDreamPowder.h"

ALootDreamPowderPillar::ALootDreamPowderPillar()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/DreamPowderPillar_Test.DreamPowderPillar_Test'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void ALootDreamPowderPillar::Break()
{
	// Destory 함수가 부모한테 있으니까
	Super::Break();
	
	// Spawn Goods!
	GetWorld()->SpawnActor<AGoodsDreamPowder>(GoodsClass, GetActorLocation(), GetActorRotation());
	
	// Effect Play
	UE_LOG(LogTemp, Warning, TEXT("ALootDreamPowderPillar"));
}
