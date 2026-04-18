
#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/GoodsBase.h"
#include "GoodsGoldCoin.generated.h"

UCLASS()
class RETRI_API AGoodsGoldCoin : public AGoodsBase
{
	GENERATED_BODY()
	
public:
	AGoodsGoldCoin();
	
protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Gold = 10;
};
