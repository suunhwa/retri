// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractSanctuary.h"

#include "ReTriGameData.h"
#include "Level/Actors/NZW_TestPlayer.h"

#include "Components/CapsuleComponent.h"

AInteractSanctuary::AInteractSanctuary()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/Sanctuary_Test.Sanctuary_Test'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
	//MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	//MeshComp->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
}

void AInteractSanctuary::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Sanctuary;
}

void AInteractSanctuary::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	if (!MyPlayer) return;
	if (MyPlayer->GD->PlayerStats.Gold < Cost) return;
	
	MyPlayer->GD->PlayerStats.CurrentHP += HealHP;
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InteractName);
}
