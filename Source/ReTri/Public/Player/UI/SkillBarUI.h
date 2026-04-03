/**
 * 스킬 퀵슬롯 UI
 * layout ( 왼쪽 -> 오른쪽 )
 * 
 * [slot dash
 */


#pragma once

#include "CoreMinimal.h"
#include "SkillSlotUI.h"
#include "Blueprint/UserWidget.h"
#include "Player/Components/AbilityComponent.h"
#include "SkillBarUI.generated.h"


UCLASS()
class RETRI_API USkillBarUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="SkillBar")
	void InitSkillBar(UAbilityComponent* InAbilityComp);
	
	// 스킬 스왑
	void HandleSlotDrop(USkillSlotUI* SourceSlot, USkillSlotUI* TargetSlot);
	
protected:
	// dash, 고정
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USkillSlotUI> SlotDash;
	
	// RMB - 게임 플레이 시 HandCannon
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USkillSlotUI> SlotRMB;
	
	// Q (게임 플레이 시 empty)
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USkillSlotUI> SlotQ;
	
	// E (게임 플레이 시 empty)
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USkillSlotUI> SlotE;
	
	// R - 게임 플레이 시 QuickTrigger
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USkillSlotUI> SlotR;
	
	// LMB - salamander powder (기본 공격 패시브 스킬), 고정
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USkillSlotUI> SlotLMB;
	
	/* salamander powder 슬롯 아이콘 */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="SkillBar|KeyIcons")
	TObjectPtr<UTexture2D> SalamanderIcon;
	
	/* 키 img 아이콘 */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="SkillBar|KeyIcons")
	TObjectPtr<UTexture2D> KeyIcon_Space;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="SkillBar|KeyIcons")
	TObjectPtr<UTexture2D> KeyIcon_RMB;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="SkillBar|KeyIcons")
	TObjectPtr<UTexture2D> KeyIcon_Q;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="SkillBar|KeyIcons")
	TObjectPtr<UTexture2D> KeyIcon_E;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="SkillBar|KeyIcons")
	TObjectPtr<UTexture2D> KeyIcon_R;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="SkillBar|KeyIcons")
	TObjectPtr<UTexture2D> KeyIcon_LMB;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityComponent> AbilityComp;
	
	// 콜백
	UFUNCTION()
	void OnSkillSlotChanged(EAbilitySlot ChangedSlot);
	
	void RefreshSlot(USkillSlotUI* SlotWidget, EAbilitySlot AbilitySlot);
	
	USkillSlotUI* FindSlotWidget(EAbilitySlot Slot) const;
};
