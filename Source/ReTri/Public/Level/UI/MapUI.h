// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapUI.generated.h"

class UMapNode;

/**
 * 
 */
UCLASS()
class RETRI_API UMapUI : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	
protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;	
	
public:
	UPROPERTY(EditAnywhere, Category = "MapUI")
	TSubclassOf<UMapNode> MapUIClass;
	
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* MapCanvas;
};
