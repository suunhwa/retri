
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectButtonUI.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectButtonClicked, int32, ButtonIndex);

UCLASS()
class RETRI_API USelectButtonUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;	
	virtual void NativeDestruct() override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnSelectButtonClicked OnSelectClicked;

	UFUNCTION()
	void OnHover();
	UFUNCTION()
	void OnClicked();
	
	UFUNCTION(BlueprintCallable)
	void SetTextTitle(FString Title, FLinearColor Color = FLinearColor(0.022000f, 0.002732f, 0.002428f, 1.000000f));
	UFUNCTION(BlueprintCallable)
	void SetTextInfo(FString Info);
	
	UPROPERTY(BlueprintReadOnly)
	int32 ThisIndex;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonSelect;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextTitle;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* HoverSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* ClickSound;
};
