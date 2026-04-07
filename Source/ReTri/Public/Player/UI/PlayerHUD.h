// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/Components/StatComponent.h"
#include "PlayerHUD.generated.h"

class UProgressBar;
class UTextBlock;
class USkillBarUI;
class UAbilityComponent;
class UCurseQuestUI;

/**
 * 플레이어 HUD (HP바, 골드, 꿈가루)
 */
UCLASS()
class RETRI_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitSkillBar(UAbilityComponent* InAbilityComp);

	// HealthComponent.OnHPChanged에 바인딩 
	UFUNCTION()
	void OnHPChanged(float CurrentHP, float MaxHP);

	// StatComponent.OnStatChanged에 바인딩
	UFUNCTION()
	void OnStatChanged(EStatTypes StatType, float NewValue);

	// StatComponent.OnExpChanged에 바인딩
	UFUNCTION()
	void OnExpChanged(int32 CurrentExp, int32 RequiredExp, int32 CurrentLevel);

protected:
	// HP 바
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> HPText;

	// EXP 바
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UProgressBar> ExpBar;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> LevelText;

	// 재화
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> GoldText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> DreamPowderText;

	// 스킬바
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<USkillBarUI> SkillBar;
	
	// 증오 퀘스트 UI
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UCurseQuestUI> CurseQuestUI;
};
