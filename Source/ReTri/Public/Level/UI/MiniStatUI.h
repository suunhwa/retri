
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniStatUI.generated.h"

class UTextBlock;

UCLASS()
class RETRI_API UMiniStatUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxHpText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AttackDamageText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AbilityPowerText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* AttackSpeedText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CoolTimeText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CriticalText;	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* FireText;		
};
