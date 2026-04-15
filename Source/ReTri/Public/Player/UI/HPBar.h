#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBar.generated.h"

class UProgressBar;
class UImage;


UCLASS()
class RETRI_API UHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	// HealthComponent.OnHPChanged에 바인딩 
	UFUNCTION()
	void OnHPChanged(float CurrentHP, float MaxHP);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HPBar;
};
