// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectButtonUI.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectButtonClicked, int32, ButtonIndex);

/**
 * 
 */
UCLASS()
class RETRI_API USelectButtonUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;	
	
public:
	// 외부에서 바인딩 할 Delegate
	UPROPERTY(BlueprintAssignable)
	FOnSelectButtonClicked OnSelectClicked;

	UFUNCTION()
	void OnClicked();
	
	UFUNCTION(BlueprintCallable)
	void SetTextTitle(FString Title, FLinearColor Color = FLinearColor(0.022000f, 0.002732f, 0.002428f, 1.000000f));
	UFUNCTION(BlueprintCallable)
	void SetTextInfo(FString Info);
	
	// 이 버튼이 몇번째인지
	UPROPERTY(BlueprintReadOnly)
	int32 ThisIndex;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonSelect;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextTitle;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextInfo;
};
