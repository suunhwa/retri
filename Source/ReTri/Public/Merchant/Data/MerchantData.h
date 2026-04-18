
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "MerchantData.generated.h" 

USTRUCT(BlueprintType)
struct FMerchantDialogueRow : public FTableRowBase 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Merchant|Dialogue")
	FString Dialogue;
};
