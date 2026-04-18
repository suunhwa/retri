
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
	AInteractRemnants();
	
	virtual void BeginPlay() override;

public:	
	virtual void Interact_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemBase> ItemClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player|Skills")
	TMap<FName, TSubclassOf<UAbilityBase>> SkillIDToClassMap;
};
