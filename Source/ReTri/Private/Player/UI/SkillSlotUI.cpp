// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/SkillSlotUI.h"

void USkillSlotUI::SetSkill(UAbilityBase* NewSkill)
{
}

FReply USkillSlotUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void USkillSlotUI::NativeOnDragDetected(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool USkillSlotUI::NativeOnDrop(const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	
}

void USkillSlotUI::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	
}

void USkillSlotUI::SetupMaterials()
{
}

void USkillSlotUI::UpdateIconDisplay()
{
}

void USkillSlotUI::UpdateCooldownDisplay(bool bOnCooldown, float Remaining, float Total)
{
}
