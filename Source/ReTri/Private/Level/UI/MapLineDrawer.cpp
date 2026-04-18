

#include "Level/UI/MapLineDrawer.h"
#include "Engine/GameInstance.h"
#include "MapSubSystem.h"

int32 UMapLineDrawer::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 ReturnLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	auto GI = GetGameInstance();
	if (!GI) return ReturnLayerId;
	auto MapSub = GI->GetSubsystem<UMapSubSystem>();
	if (!MapSub) return ReturnLayerId;
	
	FLinearColor LineColor = FLinearColor(.8f, .8f, .8f, .4f);
	float LineThickness = 2.0f;
	
	for (const FMapNodeData& Data : MapSub->CurMapDatas)
	{
		for (int32 TargetIndex : Data.ConnectMapIndexs)
		{
			if (TargetIndex > Data.MapIndex)
			{
				TArray<FVector2D> Points;
				Points.Add(Data.UIPosition);
				Points.Add(MapSub->CurMapDatas[TargetIndex].UIPosition);
				
				FSlateDrawElement::MakeLines(
					OutDrawElements,
					ReturnLayerId,
					AllottedGeometry.ToPaintGeometry(),
					Points,
					ESlateDrawEffect::None,
					LineColor,
					true,
					LineThickness
				);
			}			
		}	
	}
	
	return ReturnLayerId;
}
