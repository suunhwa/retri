// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractPortal.h"

#include "ReTriGameInstance.h"

void AInteractPortal::BeginPlay()
{
	Super::BeginPlay();
	InteractableType = EInteractableType::Portal;
}

void AInteractPortal::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	// todo: 메뉴 UI띄우기
	UE_LOG(LogTemp, Warning, TEXT("메뉴 UI띄우기"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InteractName);
	
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetGameInstance());
	GI->EnterMap(GI->CurMapIndex+1);
}
