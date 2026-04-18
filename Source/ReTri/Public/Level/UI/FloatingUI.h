
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingUI.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class RETRI_API UFloatingUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void PlayScale(FText Text, FLinearColor OutLinearColor);
	UFUNCTION()
	void PlayFloating(FText Text, FLinearColor OutLinearColor);
	UFUNCTION()
	void PlayFloatingLong(FText Text, FLinearColor OutLinearColor);
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Scale;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Floating;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* FloatingLong;
	
	UPROPERTY(meta=(BindWidget))	
	UTextBlock* ShowText;
};
