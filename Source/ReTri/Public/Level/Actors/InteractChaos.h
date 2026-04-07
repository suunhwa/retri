// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/InteractableBase.h"
#include "Level/Data/InteractableData.h"
#include "InteractChaos.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API AInteractChaos : public AInteractableBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	virtual void Interact_Implementation() override;
	
	UFUNCTION()
	void OnChaosSelected(int32 Index);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chaos")
	UDataTable* ChaosDataTable;
	
	UPROPERTY()
	USelectUI* SelectUIInstance;
	
	virtual void ShowSelectUI() override;
	virtual void HideSelectUI() override;
private:
	TArray<FChaosData*> PickedChaoses;
};