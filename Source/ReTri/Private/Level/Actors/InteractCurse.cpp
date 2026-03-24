// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractCurse.h"


AInteractCurse::AInteractCurse()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/Curse_Test.Curse_Test'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void AInteractCurse::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Curse;
}

void AInteractCurse::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	UE_LOG(LogTemp, Warning, TEXT("3가지 선택지 UI 띄우기 -> 저주를 받고 스텟 레벨업"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InteractName);
}
