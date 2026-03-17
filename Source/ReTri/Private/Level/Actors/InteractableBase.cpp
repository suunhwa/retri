// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractableBase.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BoxComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCapsuleHalfHeight(150.f);
	CapsuleComp->SetCapsuleRadius(120.f);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AInteractableBase::Interact_Implementation()
{
	// 여기에 Interact F 키 눌렀을 때 플레이어가 실행할 함수 내용 각각 적용
	
	UE_LOG(LogTemp, Warning, TEXT("InteractableBase"));
}

