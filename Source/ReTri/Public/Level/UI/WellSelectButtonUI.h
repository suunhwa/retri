// Fill out your copyright notice in the Description page of Project Settings.

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
	// 외부에서 바인딩 할 Delegate
	UPROPERTY(BlueprintAssignable)
	FOnWellSelectButtonClicked OnWellSelectClicked;

	UFUNCTION()
	void OnClicked();
	
	UFUNCTION(BlueprintCallable)
	void SetTextDescription(FString Title);
	UFUNCTION(BlueprintCallable)
	void SetTextCost(FString Info);
	
	// 이 버튼이 몇번째인지
	UPROPERTY(BlueprintReadOnly)
	int32 ThisIndex;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonSelect;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextDescription;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextCost;
};
