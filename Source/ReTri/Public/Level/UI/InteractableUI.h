// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableUI.generated.h"

class UTextBlock; 

UCLASS()
class RETRI_API UInteractableUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnInteract(const FString& Name);
	
protected:
	/** Blueprint에서 실행 할 함수 */
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnInteract"))
	void BP_Interact_UI(const FString& Name);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* InteractableName;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Big;
	
	void PlayBigAnimation();
	void ReverseBigAnimation();
};
