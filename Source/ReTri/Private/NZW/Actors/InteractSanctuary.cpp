// Fill out your copyright notice in the Description page of Project Settings.


#include "NZW/Actors/InteractSanctuary.h"

#include "Components/CapsuleComponent.h"

AInteractSanctuary::AInteractSanctuary()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/NZW/04_Assets/Meshs/Sanctuary_Test.Sanctuary_Test'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComp->GetScaledCapsuleHalfHeight()));
}

void AInteractSanctuary::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractSanctuary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractSanctuary::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	FString Type = StaticEnum<EInteractableType>()->GetNameStringByValue((int64)InteractableType);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Type);
}
