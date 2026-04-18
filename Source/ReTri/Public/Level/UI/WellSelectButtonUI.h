
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WellSelectButtonUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWellSelectButtonClicked, int32, Index);

class UTextBlock;
class UButton;

UCLASS()
class RETRI_API UWellSelectButtonUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;	
	virtual void NativeDestruct() override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnWellSelectButtonClicked OnWellSelectClicked;

	UFUNCTION()
	void OnClicked();
	UFUNCTION()
	void OnHover();
	
	UFUNCTION(BlueprintCallable)
	void SetTextDescription(FString Title);
	UFUNCTION(BlueprintCallable)
	void SetTextCost(FString Info);
	
	UPROPERTY(BlueprintReadOnly)
	int32 ThisIndex;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonSelect;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextDescription;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* HoverSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* ClickSound;
};
