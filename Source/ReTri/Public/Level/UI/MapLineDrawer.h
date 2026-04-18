
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapLineDrawer.generated.h"

UCLASS()
class RETRI_API UMapLineDrawer : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
