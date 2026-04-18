
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class USkillItemInterface : public UInterface
{
	GENERATED_BODY()
};

class RETRI_API ISkillItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void Acquire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void Hold(AActor* PlayerActor);
};