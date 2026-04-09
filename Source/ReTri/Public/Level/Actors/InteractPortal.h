// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/InteractableBase.h"
#include "InteractPortal.generated.h"

class UMapUI;

UCLASS()
class RETRI_API AInteractPortal : public AInteractableBase
{
	GENERATED_BODY()
	
protected:
	AInteractPortal();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Interact_Implementation() override;
	
	// 메뉴 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMapUI> MapUIClass;
	
	UPROPERTY()
	UMapUI* MapUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* PortalSound;
};
