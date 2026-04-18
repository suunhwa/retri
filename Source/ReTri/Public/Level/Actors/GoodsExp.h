
#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/GoodsBase.h"
#include "GoodsExp.generated.h"

UCLASS()
class RETRI_API AGoodsExp : public AGoodsBase
{
	GENERATED_BODY()
public:
	AGoodsExp();
	
protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
