
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDropItem.generated.h"

UCLASS()
class RETRI_API ABossDropItem : public AActor
{
	GENERATED_BODY()

public:
	ABossDropItem();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

public:
	UPROPERTY(EditDefaultsOnly, Category="LevelTransition")
	FName NextLevelName = NAME_None;
};
