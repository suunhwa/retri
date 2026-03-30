// Fill out your copyright notice in the Description page of Project Settings.


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
	
	// 선 설정
	FLinearColor LineColor = FLinearColor::White;
	float LineThickness = 2.0f;
	
	// 맵 데이터를 돌면서 선 그리기
	// 현재 위젯이 MapCanvas를 꽉 채우고 있으므로, Data.UIPosition을 그대로 쓰면 노드 위치와 일치하게..
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
