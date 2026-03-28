// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/MapUI.h"
#include "Level/UI/MapNode.h"
#include "Level/Data/InteractableData.h"
#include "MapSubSystem.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/GameInstance.h"

void UMapUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	auto GI = GetGameInstance();
	if (!GI) return;
	auto MapSub = GI->GetSubsystem<UMapSubSystem>();
	if (!MapSub) return;
	
	for (FMapNodeData Data : MapSub->CurMapDatas)
	{
		FString NodeName = FString::Printf(TEXT("Node_%d"), Data.MapIndex);
		UMapNode* Node = CreateWidget<UMapNode>(GetWorld(), MapUIClass, FName(*NodeName));
		Node->NodeIndexNumber = Data.MapIndex;
		for (FName Name : Data.SpawnInteractableRowNames)
		{
			if (Name.IsEqual(TEXT("Sanctuary")))
			{
				Node->SetSanctuaryVisibility(ESlateVisibility::Visible);
			}
			if (Name.IsEqual(TEXT("Well")))
			{
				Node->SetWellVisibility(ESlateVisibility::Visible);
			}
		}
		if (!Node || !MapCanvas) continue;
		
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MapCanvas->AddChild(Node));
		
		if (CanvasSlot)
		{
			CanvasSlot->SetPosition(Data.UIPosition);
			CanvasSlot->SetAutoSize(true); 
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f)); 
		}
	}
}

int32 UMapUI::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, 
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// 1. 항상 부모의 Paint를 먼저 호출해서 기본 UI들을 먼저 그리게 한 뒤 LayerId를 받습니다.
	int32 ReturnLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	
	auto GI = GetGameInstance();
	if (!GI) return ReturnLayerId;
	auto MapSub = GI->GetSubsystem<UMapSubSystem>();
	if (!MapSub) return ReturnLayerId;
	
	// 선 색상과 굵기(픽셀) 설정
	FLinearColor LineColor = FLinearColor::White;
	float LineThickness = 2.0f;
	
	// 2. 방 배열을 순회하면서 선 긋기
	for  (const FMapNodeData& Data : MapSub->CurMapDatas)
	{
		for (int32 TargetIndex : Data.ConnectMapIndexs)
		{
			// 중복 제거 -> 무조건 작은쪽에서 그리기
			if (Data.MapIndex < TargetIndex) 
			{
				// 출발점과 도착점 좌표
				FVector2D S = Data.UIPosition;
				FVector2D E = MapSub->CurMapDatas[TargetIndex].UIPosition;
				
				// 만약 가운데에서 안나타나면
				// S += FVector2D(25.f, 0.0f);
				// E += FVector2D(-25.f, 0.0f);

				// 선을 그을 두 점 확보
				TArray<FVector2D> Points;
				Points.Add(S);
				Points.Add(E);

				// 엔진의 네이티브 선 긋기 함수 호출
				FSlateDrawElement::MakeLines(
					OutDrawElements,
					ReturnLayerId,
					AllottedGeometry.ToPaintGeometry(),
					Points, // 연결할 포지션 배열
					ESlateDrawEffect::None,
					LineColor,
					true, // 안티앨리어싱(부드러운 선) 켜기
					LineThickness
				);
			}
		}
	}

	// 성공적으로 선을 덧그렸으므로 LayerId 반환
	return ReturnLayerId; 
}
