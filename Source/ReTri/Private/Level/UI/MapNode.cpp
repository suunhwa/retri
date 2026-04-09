// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/MapNode.h"

#include "ReTri/ReTri.h"
#include "MapSubSystem.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMapNode::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Node)
	{
		Node->OnClicked.AddDynamic(this, &UMapNode::OnNodeClicked);
		Node->OnHovered.AddDynamic(this, &UMapNode::OnNodeHover);
		Node->OnUnhovered.AddDynamic(this, &UMapNode::OnNodeHoverEnd);
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

void UMapNode::OnNodeHover()
{
	PlayAnimation(Hover, 0, 0);
	
	UGameplayStatics::PlaySound2D(GetWorld(), HoverSound);
}

void UMapNode::OnNodeHoverEnd()
{
	StopAnimation(Hover);
}

void UMapNode::OnNodeClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	auto MapSub = GetGameInstance()->GetSubsystem<UMapSubSystem>();
	if (!MapSub) return;
	MapSub->EnterMap(NodeIndexNumber);
	JIWONLOG("노드번호 : %d", NodeIndexNumber);
}

