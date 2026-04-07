// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurseQuestUI.generated.h"

class UBackgroundBlur;
class UVerticalBox;
class UFont;

UCLASS()
class RETRI_API UCurseQuestUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	// Blur 
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBackgroundBlur> BackBlur;
	
	// VerticalBox 
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> QuestVerticalBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Font")
	TObjectPtr<UFont> CustomFontAsset; 
	
	UFUNCTION(BlueprintCallable)
	void UpdateCurseQuestText();
};
