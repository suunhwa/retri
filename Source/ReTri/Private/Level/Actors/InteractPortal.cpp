// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractPortal.h"

#include "Blueprint/UserWidget.h"
#include "ReTriGameInstance.h"
#include "Level/UI/MapUI.h"

void AInteractPortal::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Portal;
	
	MapUI = CreateWidget<UMapUI>(GetWorld(), MapUIClass);

}

void AInteractPortal::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	if (MapUI != nullptr)
	{
		MapUI->AddToViewport(10);
	}
	
	// todo: 메뉴 UI띄우기
	UE_LOG(LogTemp, Warning, TEXT("메뉴 UI띄우기"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InteractName);
	
	// UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetGameInstance());
	// GI->EnterMap(GI->CurMapIndex+1);
}
