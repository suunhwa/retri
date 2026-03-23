// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/NoExportTypes.h"
#include "AbilityBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooldownChanged, float, Remaining, float, Total);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityActivated);

/**
 *  플레이어 스킬 기반 클래스
 *  자식 클래스는 CanActivate() & Activate()만 구현
 */
UCLASS(Abstract, Blueprintable)
class RETRI_API UAbilityBase : public UObject
{
	GENERATED_BODY()
	
public:
	bool TryActivate(ACharacter* Owner);
	
	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnCooldownChanged OnCooldownChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnAbilityActivated OnAbilityActivated;
	
	UFUNCTION(BlueprintPure, Category="Ability")
	bool IsOnCooldown() const { return bIsOnCooldown; }
	
	UFUNCTION(BlueprintPure, Category="Ability")
	float GetRemainingCooldown() const { return RemainingCooldown; }
	
	UFUNCTION(BlueprintPure, Category="Ability")
	float GetTotalCooldown() const { return Cooldown; }
	
	// 외부에서 쿨다운 수치 변경 시 호출
	void SetCooldown(float NewCooldown) { Cooldown = FMath::Max(0.1f, NewCooldown); }
	
protected:
	virtual bool CanActivate(ACharacter* Owner) { return true; }
	virtual void Activate(ACharacter* Owner) PURE_VIRTUAL(UAbilityBase::Activate, );
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	float Cooldown = 1.f;

	// 아이콘 — 스킬 UI 슬롯에 표시
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TObjectPtr<UTexture2D> Icon;

private:
	void StartCooldown(UWorld* World);
	void TickCooldown();

	bool bIsOnCooldown = false;
	float RemainingCooldown = 0.f;
	FTimerHandle CooldownTimer;
};
