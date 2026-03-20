// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractChaos.h"


AInteractChaos::AInteractChaos()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/Chaos_Test.Chaos_Test'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void AInteractChaos::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Chaos;
}

void AInteractChaos::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	UE_LOG(LogTemp, Warning, TEXT("스탯 선택하는 UI 띄우고 선택하면 해당 스탯 UP!!"));
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InteractName);
}
