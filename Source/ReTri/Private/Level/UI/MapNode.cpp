// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/MapNode.h"

#include "ReTri/ReTri.h"
#include "MapSubSystem.h"
#include "Components/Button.h"

void UMapNode::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Node)
	{
		Node->OnClicked.AddDynamic(this, &UMapNode::OnNodeClicked);
	}
	
	// auto GI = GetGameInstance();
	// if (!GI) return;
	auto MapSub = GetGameInstance()->GetSubsystem<UMapSubSystem>();
	if (!MapSub) return;
	
	if (NodeIndexNumber == MapSub->CurMapIndex)
		CurUI->SetVisibility(ESlateVisibility::Visible);
	else
		CurUI->SetVisibility(ESlateVisibility::Hidden);
}

void UMapNode::OnNodeClicked()
{
	auto MapSub = GetGameInstance()->GetSubsystem<UMapSubSystem>();
	if (!MapSub) return;
	MapSub->EnterMap(NodeIndexNumber);
	JIWONLOG("노드번호 : %d", NodeIndexNumber);
}

