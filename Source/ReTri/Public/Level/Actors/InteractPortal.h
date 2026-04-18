
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
	
	virtual void BeginPlay() override;

public:	
	virtual void Interact_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMapUI> MapUIClass;
	
	UPROPERTY()
	UMapUI* MapUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* PortalSound;
};
