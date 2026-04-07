// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/InteractableBase.h"
#include "Player/Abilities/AbilityBase.h"
#include "InteractRemnants.generated.h"

class AItemBase;

UCLASS()
class RETRI_API AInteractRemnants : public AInteractableBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Interact_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemBase> ItemClass;
	
	// SkillID(DataTable Row Key) → ability class 매핑
	// BP에서 획득 스킬(Acquired) 목록 추가해야됩니당
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player|Skills")
	TMap<FName, TSubclassOf<UAbilityBase>> SkillIDToClassMap;
};
