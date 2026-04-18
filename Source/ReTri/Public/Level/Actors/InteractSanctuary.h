
#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/InteractableBase.h"
#include "InteractSanctuary.generated.h"

UCLASS()
class RETRI_API AInteractSanctuary : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AInteractSanctuary();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Interact_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* HealStaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealHP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost = 30;
};
