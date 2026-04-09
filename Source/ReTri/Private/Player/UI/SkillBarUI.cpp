// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/UI/SkillBarUI.h"

#include "Player/UI/SkillSlotUI.h"
#include "Player/Components/AbilityComponent.h"
#include "Player/Abilities/AbilityBase.h"
#include "Player/Abilities/DashAbility.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/Character.h"

void USkillBarUI::InitSkillBar(UAbilityComponent* InAbilityComp)
{
	AbilityComp = InAbilityComp;

	if (AbilityComp)
	{
		AbilityComp->OnSkillSlotChanged.AddDynamic(this, &USkillBarUI::OnSkillSlotChanged);
	
		// 플레이어 기본공격 카운터 바인딩 (샐러맨더 가루 UI)
		if (APlayerCharacter* PC = Cast<APlayerCharacter>(AbilityComp->GetOwner()))
		{
			PC->OnAttackCountChanged.AddDynamic(this, &USkillBarUI::OnAttackCountChanged);
		}
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

		// 충전 횟수 표시 바인딩
		if (UDashAbility* Dash = Cast<UDashAbility>(AbilityComp ? AbilityComp->GetAbility(EAbilitySlot::Dash) : nullptr))
		{
			Dash->OnChargeChanged.AddDynamic(this, &USkillBarUI::OnDashChargeChanged);
			SlotDash->UpdateStackDisplay(Dash->GetCurrentCharges(), Dash->GetMaxCharges());
		}
	}

	// ---- 핸드캐논 (RMB) — 초기 장착, 교체 가능 ----
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

	// ---- 빠른손 (R) — 초기 장착, 교체 가능 ----
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
	if (!AbilityComp || !SourceSlot) return;

	const EAbilitySlot SrcSlot = SourceSlot->GetLinkedSlot();

	// TargetSlot == nullptr : 스킬바 밖으로 드래그 → 바닥에 드롭 후 슬롯 비움
	if (!TargetSlot)
	{
		FVector DropLocation = AbilityComp->GetOwner()->GetActorLocation();
		AbilityComp->DropSkill(SrcSlot, DropLocation);
		return;
	}

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

void USkillBarUI::OnAttackCountChanged(int32 Count)
{
	// 4타째 발사 후 Count가 0으로 리셋됨 → 0이면 빈 상태
	if (SlotLMB)
	{
		SlotLMB->UpdateStackDisplay(Count, 4);
	}
}

void USkillBarUI::OnDashChargeChanged(int32 Current, int32 Max)
{
	if (SlotDash)
	{
		SlotDash->UpdateStackDisplay(Current, Max);
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
