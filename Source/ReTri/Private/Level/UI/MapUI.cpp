// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/MapUI.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Level/UI/MapNode.h"
#include "Level/UI/MapLineDrawer.h"
#include "Level/Data/InteractableData.h"
#include "MapSubSystem.h"
#include "ReTriGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ReTri/ReTri.h"

void UMapUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetPause(true);
		
		// FInputModeUIOnly InputMode;
		// InputMode.SetWidgetToFocus(TakeWidget()); // 현재 위젯에 포커스
		// PC->SetInputMode(InputMode);
	}
	
	auto GI = Cast<UReTriGameInstance>(GetGameInstance());
	if (!GI) return;
	auto MapSub = GI->GetSubsystem<UMapSubSystem>();
	if (!MapSub) return;
	
	// 선 그리기 -> 아래 깔리게 하기 위해
	if (MapCanvas && MapLineClass)
	{
		UMapLineDrawer* Line = CreateWidget<UMapLineDrawer>(GetWorld(), MapLineClass);
		if (Line)
		{
			UCanvasPanelSlot* LineSlot = MapCanvas->AddChildToCanvas(Line);
			if (LineSlot)
			{
				LineSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
				LineSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
				LineSlot->SetZOrder(0); // 노드(11)보다 아래에 배치
			}
		}
		else
		{
			// JIWONLOG("[UMapUI] 에러: MapLineClass가 할당되지 않음");
		}
	}
	
	for (FMapNodeData Data : MapSub->CurMapDatas)
	{
		if (!MapCanvas || !MapUIClass) break;
		
		FString NodeName = FString::Printf(TEXT("Node_%d"), Data.MapIndex);
		UMapNode* MapNode = CreateWidget<UMapNode>(GetWorld(), MapUIClass, FName(*NodeName));
		if (!MapNode) continue;
		
		MapNode->NodeIndexNumber = Data.MapIndex;
		
		FName EnumName = FName(*StaticEnum<EMapNodeType>()->GetNameStringByValue((int64)Data.MapType));
		FMapUIData* UIData = GI->MapUIData->FindRow<FMapUIData>(EnumName, TEXT("MapNode"));
		if (!UIData)
		{
			// JIWONLOG("[MapUI] UIData를 찾지 못함: %s", *EnumName.ToString());
			continue;
		}
		
		FButtonStyle Style;
		Style.Normal.SetResourceObject(UIData->MapIcon);
		Style.Normal.ImageSize = FVector2D(50.f, 50.f);
		Style.Hovered = Style.Normal;
		Style.Pressed = Style.Normal;
		FLinearColor MainColor = UIData->IconColor;
		Style.Normal.TintColor = MainColor;
		MainColor += FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
		Style.Hovered.TintColor = MainColor;
		MainColor -= FLinearColor(0.8f, 0.8f, 0.8f, 1.0f);
		Style.Pressed.TintColor = MainColor;	

		MapNode->Node->SetStyle(Style);
		
		for (auto Name : Data.SpawnInteractableRowNames)
		{
			if (Name.Key.IsEqual(TEXT("Sanctuary")))
			{
				MapNode->SetSanctuaryVisibility(ESlateVisibility::Visible);
			}
			else if (Name.Key.IsEqual(TEXT("Well")))
			{
				MapNode->SetWellVisibility(ESlateVisibility::Visible);
			}
		}

		if (UCanvasPanelSlot* CanvasSlot = MapCanvas->AddChildToCanvas(MapNode))
		{
			CanvasSlot->SetPosition(Data.UIPosition);
			CanvasSlot->SetZOrder(11);
			CanvasSlot->SetAutoSize(true); 
			CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f)); 
		}
	}
}

void UMapUI::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetPause(false);
		
		// FInputModeUIOnly InputMode;
		// InputMode.SetWidgetToFocus(TakeWidget()); // 현재 위젯에 포커스
		// PC->SetInputMode(InputMode);
	}
}
