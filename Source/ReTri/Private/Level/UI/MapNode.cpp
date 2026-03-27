// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/MapNode.h"

#include "ReTriGameInstance.h"
#include "Components/Button.h"
#include "ReTri/ReTri.h"

void UMapNode::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Node)
	{
		Node->OnClicked.AddDynamic(this, &UMapNode::OnNodeClicked);
	}
}

void UMapNode::OnNodeClicked()
{
	auto* GI = Cast<UReTriGameInstance>(GetGameInstance());
	GI->EnterMap(NodeIndexNumber);
	SCREENLOG("노드번호 : %d", NodeIndexNumber);
}
