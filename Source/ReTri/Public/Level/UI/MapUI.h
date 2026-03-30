// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapUI.generated.h"

class UMapNode;
class UMapLineDrawer;

/**
 * 
 */
UCLASS()
class RETRI_API UMapUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;	
	
	//virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;	
	
public:
	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* MapCanvas;
	
	UPROPERTY(EditAnywhere, Category = "MapUI")
	TSubclassOf<UMapNode> MapUIClass;
	
	UPROPERTY(EditAnywhere, Category = "MapUI")
	TSubclassOf<UMapLineDrawer> MapLineClass;
	
	//UPROPERTY()
	//TArray<UMapNode*> NodeWidgets;
};


