// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/UI/SkillSlotUI.h"

#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Player/Abilities/AbilityBase.h"
#include "Player/UI/SkillBarUI.h"
#include "Player/UI/SkillDragDrop.h"

void USkillSlotUI::NativeConstruct()
{
	Super::NativeConstruct();
	SetupMaterials();
}

void USkillSlotUI::InitSlot(EAbilitySlot InSlot, bool bInFixed, UTexture2D* InKeyIcon,
                            UAbilityBase* InAbility, USkillBarUI* InOwningBar,
                            UTexture2D* InFallbackIcon)
{
	LinkedSlot = InSlot;
	bFixed = bInFixed;
	OwningBar = InOwningBar;
	FallbackIcon = InFallbackIcon;

	// 키 아이콘 이미지 설정
	if (KeyImg && InKeyIcon)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(InKeyIcon);
		Brush.DrawAs = ESlateBrushDrawType::Image;
		KeyImg->SetBrush(Brush);
	}

	SetSkill(InAbility);
}

// ============================================================
//  SetAbility
// ============================================================

void USkillSlotUI::SetSkill(UAbilityBase* NewAbility)
{
	UnbindCooldown();

	LinkedSkill = NewAbility;
	UpdateIconDisplay();

	if (NewAbility)
	{
		const bool bOnCD = NewAbility->IsOnCooldown();
		UpdateCooldownDisplay(bOnCD,
		                      bOnCD ? NewAbility->GetRemainingCooldown() : 0.f,
		                      NewAbility->GetTotalCooldown());

		NewAbility->OnCooldownChanged.AddDynamic(this, &USkillSlotUI::OnCooldownChanged);
	}
	else
	{
		UpdateCooldownDisplay(false, 0.f, 1.f);
	}
}

// ============================================================
//  Drag & Drop
// ============================================================

FReply USkillSlotUI::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                             const FPointerEvent& InMouseEvent)
{
	if (bFixed)
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void USkillSlotUI::NativeOnDragDetected(const FGeometry& InGeometry,
                                        const FPointerEvent& InMouseEvent,
                                        UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	USkillDragDrop* Op = NewObject<USkillDragDrop>(this);
	Op->SourceSlot = this;
	Op->Pivot = EDragPivot::MouseDown;

	if (IconImg)
	{
		UImage* DragVisual = NewObject<UImage>(this);
		DragVisual->SetBrush(IconImg->GetBrush());
		Op->DefaultDragVisual = DragVisual;
	}

	OutOperation = Op;
}

bool USkillSlotUI::NativeOnDrop(const FGeometry& InGeometry,
                                const FDragDropEvent& InDragDropEvent,
                                UDragDropOperation* InOperation)
{
	USkillDragDrop* SkillOp = Cast<USkillDragDrop>(InOperation);
	if (!SkillOp || !SkillOp->SourceSlot || SkillOp->SourceSlot == this)
		return false;

	if (bFixed)
		return false;

	if (OwningBar)
	{
		OwningBar->HandleSlotDrop(SkillOp->SourceSlot, this);
		return true;
	}

	return false;
}

void USkillSlotUI::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
                                         UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	// 슬롯 밖에 드랍 → 스킬 제거
	if (!bFixed && OwningBar)
		OwningBar->HandleSlotDrop(this, nullptr);
}

void USkillSlotUI::OnCooldownChanged(float Remaining, float Total)
{
	UpdateCooldownDisplay(Remaining > 0.f, Remaining, Total);
}

void USkillSlotUI::SetupMaterials()
{
	// ---- 아이콘 원형 MID ----
	if (IconImg && MIconCircle)
	{
		IconMI = UMaterialInstanceDynamic::Create(MIconCircle, this);
		IconImg->SetBrushFromMaterial(IconMI);
	}

	// ---- 쿨다운 라디알 MID ----
	if (CooldownOverlayImg && MCooldownRadial)
	{
		CooldownMI = UMaterialInstanceDynamic::Create(MCooldownRadial, this);
		CooldownOverlayImg->SetBrushFromMaterial(CooldownMI);
	}

	// ---- 테두리 glow MID ----
	if (BorderImg && MBorderGlow)
	{
		BorderMI = UMaterialInstanceDynamic::Create(MBorderGlow, this);
		BorderImg->SetBrushFromMaterial(BorderMI);
	}
}

void USkillSlotUI::UpdateIconDisplay()
{
	if (!IconImg) return;

	UTexture2D* IconTex = nullptr;
	if (LinkedSkill)
		IconTex = LinkedSkill->GetIcon();
	if (!IconTex)
		IconTex = FallbackIcon;

	if (IconMI)
	{
		IconMI->SetTextureParameterValue(TEXT("IconTexture"), IconTex);
	}
}

void USkillSlotUI::UpdateCooldownDisplay(bool bOnCooldown, float Remaining, float Total)
{
	// ---- 라디알 쿨다운 재질 파라미터 갱신 ----
	// CooldownRatio: 1.0 = 막 발동(완전히 어두운 원), 0.0 = 준비완료(투명)
	const float Ratio = (Total > 0.f) ? FMath::Clamp(Remaining / Total, 0.f, 1.f) : 0.f;

	if (CooldownMI)
	{
		CooldownMI->SetScalarParameterValue(TEXT("CooldownRatio"), Ratio);
	}

	// 재질 없으면 Visibility로 폴백
	if (!CooldownMI && CooldownOverlayImg)
	{
		CooldownOverlayImg->SetVisibility(bOnCooldown
			                                  ? ESlateVisibility::HitTestInvisible
			                                  : ESlateVisibility::Collapsed);
	}

	// ---- 테두리 glow: 준비완료면 1, 쿨다운 중이면 0 ----
	if (BorderMI)
	{
		BorderMI->SetScalarParameterValue(TEXT("IsReady"), bOnCooldown ? 0.f : 1.f);
	}

	// ---- 남은 초 텍스트 ----
	if (CooldownText)
	{
		if (bOnCooldown && Remaining >= 0.1f)
		{
			// 1초 이하면 소수점 1자리, 이상이면 정수
			const FText TimeText = (Remaining < 1.f)
				                       ? FText::AsNumber(FMath::RoundToFloat(Remaining * 10.f) / 10.f)
				                       : FText::AsNumber(FMath::CeilToInt(Remaining));
			CooldownText->SetText(TimeText);
			CooldownText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			CooldownText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void USkillSlotUI::UnbindCooldown()
{
	if (LinkedSkill &&
		LinkedSkill->OnCooldownChanged.IsAlreadyBound(this, &USkillSlotUI::OnCooldownChanged))
	{
		LinkedSkill->OnCooldownChanged.RemoveDynamic(this, &USkillSlotUI::OnCooldownChanged);
	}
}
