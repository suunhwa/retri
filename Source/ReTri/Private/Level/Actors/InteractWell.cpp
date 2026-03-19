// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractWell.h"

#include "Components/CapsuleComponent.h"

AInteractWell::AInteractWell()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/Wall_Test.Wall_Test'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
	//MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	//MeshComp->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
}

void AInteractWell::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Well;
}

void AInteractWell::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	UE_LOG(LogTemp, Warning, TEXT("스킬 Level Up! UI"));
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InteractName);
}
