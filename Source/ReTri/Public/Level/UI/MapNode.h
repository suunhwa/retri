// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapNode.generated.h"

class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class RETRI_API UMapNode : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION()
	void OnNodeClicked();
	
	UPROPERTY(meta = (BindWidget))
	UImage* Sanctuary;
	UPROPERTY(meta = (BindWidget))
	UImage* Well;
	
	UPROPERTY(meta = (BindWidget))
	UButton* Node;
	
	UPROPERTY(BlueprintReadOnly)
	int32 NodeIndexNumber; 
};
