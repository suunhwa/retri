// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/InteractableBase.h"
#include "InteractSanctuary.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API AInteractSanctuary : public AInteractableBase
{
	GENERATED_BODY()
public:
	AInteractSanctuary();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Interact_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost = 30;
};
