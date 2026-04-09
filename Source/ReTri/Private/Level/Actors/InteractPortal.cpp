// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractPortal.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Level/UI/MapUI.h"
#include "ReTri/ReTri.h"


AInteractPortal::AInteractPortal()
{
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagara(TEXT("/Game/Portals/Rounded/WindPortal/NS_Portal.NS_Portal"));
	if (TempNiagara.Succeeded()) NiagaraComp->SetAsset(TempNiagara.Object);
	NiagaraComp->bAutoActivate = false;
}

void AInteractPortal::BeginPlay()
{
	Super::BeginPlay();
	
	// InteractableType = EInteractableType::Portal;
	
	NiagaraComp->Activate();
	
	MapUI = CreateWidget<UMapUI>(GetWorld(), MapUIClass);
}

void AInteractPortal::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	if (MapUI != nullptr)
	{
		MapUI->AddToViewport(10);
		UGameplayStatics::PlaySound2D(GetWorld(), SelectSound);
	}
	
	UE_LOG(jiwon, Warning, TEXT("메뉴 UI띄우기"));
	UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
}
