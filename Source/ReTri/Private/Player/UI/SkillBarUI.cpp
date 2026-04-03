// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/SkillBarUI.h"
#include "Player/Abilities/AbilityBase.h"
#include "Player/Components/AbilityComponent.h"
#include "Player/UI/SkillSlotUI.h"

void USkillBarUI::InitSkillBar(UAbilityComponent* InAbilityComp)
{
	AbilityComp = InAbilityComp;
	
	if (AbilityComp)
	{
		AbilityComp->OnSkillSlotChanged.AddDynamic(this, &USkillBarUI::OnSkillSlotChanged);
	}
	
	// dash
	if (SlotDash)
	{
		
	}
}

void USkillBarUI::HandleSlotDrop(USkillSlotUI* SourceSlot, USkillSlotUI* TargetSlot)
{
	
}

void USkillBarUI::OnSkillSlotChanged(EAbilitySlot ChangedSlot)
{
	
}

void USkillBarUI::RefreshSlot(USkillSlotUI* SlotWidget, EAbilitySlot AbilitySlot)
{
	
}

USkillSlotUI* USkillBarUI::FindSlotWidget(EAbilitySlot Slot) const
{

}
