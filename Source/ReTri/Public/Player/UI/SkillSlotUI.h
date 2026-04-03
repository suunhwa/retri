/**
* 개별 skill slot widget
 * 
 * bp에서 바인딩 해야 함:
 *	IconImg (Texture2D) - skill icon
 *	CooldownOverlayImage (UImage) - radial cooldown
 *	BorderImg (UImage) - 테두리 glow
 *	KeyImg (UImage)
 */

#pragma once

#include "CoreMinimal.h"
#include "SkillBarUI.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Player/Components/AbilityComponent.h"
#include "SkillSlotUI.generated.h"

UCLASS()
class RETRI_API USkillSlotUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="SkillSlot")
	void InitSlot(EAbilitySlot InSlot,
	              bool bInFixed,
	              UTexture2D* InKeyIcon,
	              UAbilityBase* InAbility,
	              USkillBarUI* InOwningBar,
	              UTexture2D* InFallbackIcon = nullptr);

	// 스킬 slot 교체 + UI 갱신
	UFUNCTION(BlueprintCallable, Category="SkillSlot")
	void SetSkill(UAbilityBase* NewSkill);

	UFUNCTION(BlueprintPure, Category="SkillSlot")
	EAbilitySlot GetLinkedSlot() const { return LinkedSlot; }

	UFUNCTION(BlueprintPure, Category="SkillSlot")
	bool IsFixed() const { return bFixed; }

	UFUNCTION(BlueprintPure, Category="SkillSlot")
	UAbilityBase* GetLinkedSkill() const { return LinkedSkill; }

protected:
	// skill icon
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> IconImg;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> CooldownOverlayImg;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> BorderImg;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> KeyImg;

	UPROPERTY(EditDefaultsOnly, Category="SkillSlot|Materials")
	TObjectPtr<UMaterialInterface> MCooldownRadial;

	UPROPERTY(EditDefaultsOnly, Category="SkillSlot|Materials")
	TObjectPtr<UMaterialInterface> MBorderGlow;

	/* Drag & Drop */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry,
	                                  const FPointerEvent& InMouseEvent,
	                                  UDragDropOperation*& OutOperation) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	EAbilitySlot LinkedSlot = EAbilitySlot::SkillQ;
	bool bFixed = false;
	
	UPROPERTY()
	TObjectPtr<UAbilityBase> LinkedSkill;
	
	UPROPERTY()
	TObjectPtr<USkillBarUI> OwningBar;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> FallbackIcon;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CooldownMI;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BorderMI;
	
public:
	void SetupMaterials();
	void UpdateIconDisplay();
	void UpdateCooldownDisplay(bool bOnCooldown, float Remaining, float Total);
	void UnbindCooldown();
	
	UFUNCTION()
	void OnCooldownChanged(float Remaining, float Total);
};
