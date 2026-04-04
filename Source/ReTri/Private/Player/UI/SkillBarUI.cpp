// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/UI/SkillBarUI.h"

#include "Player/UI/SkillSlotUI.h"
#include "Player/Components/AbilityComponent.h"
#include "Player/Abilities/AbilityBase.h"

void USkillBarUI::InitSkillBar(UAbilityComponent* InAbilityComp)
{
	AbilityComp = InAbilityComp;

	if (AbilityComp)
	{
		AbilityComp->OnSkillSlotChanged.AddDynamic(this, &USkillBarUI::OnSkillSlotChanged);
	}

	// ---- 대시 슬롯 (소형, 왼쪽) ----
	if (SlotDash)
	{
		SlotDash->InitSlot(
			EAbilitySlot::Dash,
			/*bFixed=*/true,
			KeyIcon_Space,
			AbilityComp ? AbilityComp->GetAbility(EAbilitySlot::Dash) : nullptr,
			/*OwningBar=*/this
		);
	}

	// ---- 핸드캐논 (RMB) ----
	if (SlotRMB)
	{
		SlotRMB->InitSlot(
			EAbilitySlot::TravelerMemory1,
			/*bFixed=*/false,
			KeyIcon_RMB,
			AbilityComp ? AbilityComp->GetAbility(EAbilitySlot::TravelerMemory1) : nullptr,
			/*OwningBar=*/this
		);
	}

	// ---- Q 획득스킬 ----
	if (SlotQ)
	{
		SlotQ->InitSlot(
			EAbilitySlot::SkillQ,
			/*bFixed=*/false,
			KeyIcon_Q,
			AbilityComp ? AbilityComp->GetAbility(EAbilitySlot::SkillQ) : nullptr,
			/*OwningBar=*/this
		);
	}

	// ---- E 획득스킬 ----
	if (SlotE)
	{
		SlotE->InitSlot(
			EAbilitySlot::SkillE,
			/*bFixed=*/false,
			KeyIcon_E,
			AbilityComp ? AbilityComp->GetAbility(EAbilitySlot::SkillE) : nullptr,
			/*OwningBar=*/this
		);
	}

	// ---- 빠른손 (R) ----
	if (SlotR)
	{
		SlotR->InitSlot(
			EAbilitySlot::TravelerMemory2,
			/*bFixed=*/false,
			KeyIcon_R,
			AbilityComp ? AbilityComp->GetAbility(EAbilitySlot::TravelerMemory2) : nullptr,
			/*OwningBar=*/this
		);
	}

	// ---- 기본공격 (오른쪽, 고정) ----
	if (SlotLMB)
	{
		SlotLMB->InitSlot(
			EAbilitySlot::Dash,
			/*bFixed=*/true,
			KeyIcon_LMB,
			/*Ability=*/nullptr,
			/*OwningBar=*/this,
			SalamanderIcon
		);
	}
}

void USkillBarUI::HandleSlotDrop(USkillSlotUI* SourceSlot, USkillSlotUI* TargetSlot)
{
	if (!AbilityComp || !SourceSlot || !TargetSlot) return;

	const EAbilitySlot SrcSlot = SourceSlot->GetLinkedSlot();
	const EAbilitySlot DstSlot = TargetSlot->GetLinkedSlot();

	UAbilityBase* SrcAbility = SourceSlot->GetLinkedSkill();
	UAbilityBase* DstAbility = TargetSlot->GetLinkedSkill();

	TSubclassOf<UAbilityBase> SrcClass = SrcAbility ? SrcAbility->GetClass() : nullptr;
	TSubclassOf<UAbilityBase> DstClass = DstAbility ? DstAbility->GetClass() : nullptr;

	// SetSkill 내부에서 OnSkillSlotChanged 브로드캐스트 → UI 자동 갱신
	AbilityComp->SetSkill(DstSlot, SrcClass);
	AbilityComp->SetSkill(SrcSlot, DstClass);
}

void USkillBarUI::OnSkillSlotChanged(EAbilitySlot ChangedSlot)
{
	USkillSlotUI* SlotWidget = FindSlotWidget(ChangedSlot);
	if (SlotWidget)
	{
		RefreshSlot(SlotWidget, ChangedSlot);
	}
}

void USkillBarUI::RefreshSlot(USkillSlotUI* SlotWidget, EAbilitySlot AbilitySlot)
{
	if (!SlotWidget || !AbilityComp) return;
	SlotWidget->SetSkill(AbilityComp->GetAbility(AbilitySlot));
}

USkillSlotUI* USkillBarUI::FindSlotWidget(EAbilitySlot AbilitySlot) const
{
	switch (AbilitySlot)
	{
	case EAbilitySlot::Dash: return SlotDash;
	case EAbilitySlot::TravelerMemory1: return SlotRMB;
	case EAbilitySlot::SkillQ: return SlotQ;
	case EAbilitySlot::SkillE: return SlotE;
	case EAbilitySlot::TravelerMemory2: return SlotR;
	default: return nullptr;
	}
}
