// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractRemnants.h"

#include "Components/CapsuleComponent.h"

AInteractRemnants::AInteractRemnants()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/Remnants_Test.Remnants_Test'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
	// MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	// MeshComp->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
}

void AInteractRemnants::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Remnants;
}

void AInteractRemnants::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	UE_LOG(LogTemp, Warning, TEXT("스킬을 랜덤으로 스폰"));
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InteractName);
}
