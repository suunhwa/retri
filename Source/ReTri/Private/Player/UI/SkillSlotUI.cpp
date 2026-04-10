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
	LinkedSlot   = InSlot;
	bFixed       = bInFixed;
	OwningBar    = InOwningBar;
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
	Op->Pivot      = EDragPivot::MouseDown;

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

	// ---- 쿨다운 radial MID (테두리) ----
	if (CooldownOverlayImg && MCooldownRadial)
	{
		CooldownMI = UMaterialInstanceDynamic::Create(MCooldownRadial, this);
		CooldownOverlayImg->SetBrushFromMaterial(CooldownMI);
	}

	// ---- 쿨다운 내부 채움 MID ----
	if (CooldownFillImg)
	{
		CooldownFillImg->SetVisibility(ESlateVisibility::Collapsed);
		if (MCooldownFill)
		{
			CooldownFillMI = UMaterialInstanceDynamic::Create(MCooldownFill, this);
			CooldownFillImg->SetBrushFromMaterial(CooldownFillMI);
		}
	}

	/*// ---- 테두리 링 MID ----
	if (BorderImg && MCooldownRadial)
	{
		BorderMI = UMaterialInstanceDynamic::Create(MCooldownRadial, this);
		BorderImg->SetBrushFromMaterial(BorderMI);
	}*/
}

void USkillSlotUI::UpdateIconDisplay()
{
	if (!IconImg) return;

	UTexture2D* IconTex = nullptr;
	if (LinkedSkill)
		IconTex = LinkedSkill->GetIcon();
	if (!IconTex)
		IconTex = FallbackIcon;

	// 텍스처가 없으면 아이콘 자체를 숨김 (null을 material에 넘기면 이전 텍스처가 잔류함)
	if (!IconTex)
	{
		IconImg->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	IconImg->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	if (IconMI)
	{
		IconMI->SetTextureParameterValue(TEXT("IconTexture"), IconTex);
	}
}

void USkillSlotUI::UpdateCooldownDisplay(bool bOnCooldown, float Remaining, float Total)
{
	// ---- 라디알 쿨다운 재질 파라미터 갱신 ----
	// CooldownRatio: 1.0 = 막 발동(완전히 어두운 원), 0.0 = 준비완료(투명)
	const float Ratio = (bOnCooldown && Total > 0.f) ? FMath::Clamp(1.f - Remaining / Total, 0.f, 1.f) : 0.f;

	if (CooldownMI)
	{
		CooldownMI->SetScalarParameterValue(TEXT("CooldownRatio"), Ratio);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CooldownMI is NULL"));
	}

	if (CooldownFillImg)
	{
		if (bOnCooldown)
		{
			CooldownFillImg->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (CooldownFillMI)
				CooldownFillMI->SetScalarParameterValue(TEXT("CooldownRatio"), Ratio);
		}
		else
		{
			CooldownFillImg->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// 재질 없으면 Visibility로 폴백
	if (!CooldownMI && CooldownOverlayImg)
	{
		CooldownOverlayImg->SetVisibility(bOnCooldown ? ESlateVisibility::HitTestInvisible
		                                               : ESlateVisibility::Collapsed);
	}

	/*// ---- 테두리 쿨다운 링 ----
	if (BorderMI)
	{
		BorderMI->SetScalarParameterValue(TEXT("CooldownRatio"), Ratio);
	}*/

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

void USkillSlotUI::UpdateStackDisplay(int32 Current, int32 Max)
{
	if (Max <= 0) return;

	// 라디알 오버레이로 스택 채움 표시 (1/4→25%, 2/4→50%, 3/4→75%, 0/4→0%)
	const float Ratio = FMath::Clamp(static_cast<float>(Current) / static_cast<float>(Max), 0.f, 1.f);

	// CooldownMI와 동일한 파라미터 사용 (머티리얼 재활용)
	if (CooldownMI)
	{
		CooldownMI->SetScalarParameterValue(TEXT("CooldownRatio"), Ratio);
	}

	if (CooldownFillImg)
	{
		if (Current > 0)
		{
			CooldownFillImg->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (CooldownFillMI)
				CooldownFillMI->SetScalarParameterValue(TEXT("CooldownRatio"), Ratio);
		}
		else
		{
			CooldownFillImg->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// 텍스트로도 표시 (선택)
	if (CooldownText)
	{
		if (Current > 0)
		{
			CooldownText->SetText(FText::AsNumber(Current));
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
