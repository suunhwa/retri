
#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/GoodsBase.h"
#include "GoodsDreamPowder.generated.h"

UCLASS()
class RETRI_API AGoodsDreamPowder : public AGoodsBase
{
	GENERATED_BODY()
public:
	AGoodsDreamPowder();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DreamPower = 30;
};

