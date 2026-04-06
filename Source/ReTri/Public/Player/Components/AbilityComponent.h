// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReTriGameInstance.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "AbilityComponent.generated.h"

class UAbilityBase;

UENUM(BlueprintType)
enum class EAbilitySlot : uint8
{
	Dash,
	TravelerMemory1,  // 우클릭 고유스킬 (핸드 캐논)
	SkillQ,           // Q 획득스킬
	SkillE,           // E 획득스킬
	TravelerMemory2,  // R 고유스킬 (빠른 손)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillSlotChanged, EAbilitySlot, ChangedSlot);

UCLASS(ClassGroup=(Player), meta=(BlueprintSpawnableComponent))
class RETRI_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnSkillSlotChanged OnSkillSlotChanged;

	//
	UFUNCTION(BlueprintCallable, Category="Ability")
	bool TryActivate(EAbilitySlot Slot);
	
	// UI에서 슬롯별 스킬 오브젝트 접근
	UFUNCTION(BlueprintCallable, Category="Ability")
	UAbilityBase* GetAbility(EAbilitySlot Slot) const;
	
	// GI에 스킬 데이터 저장
	UFUNCTION(BlueprintCallable, Category="Ability")
	void SetSkill(EAbilitySlot Slot, TSubclassOf<UAbilityBase> AbilityClass);

	// 빈 슬롯(Q 또는 E)에 스킬 장착 — 상점 구매 시 호출
	UFUNCTION(BlueprintCallable, Category="Ability")
	bool EquipAcquiredSkill(TSubclassOf<UAbilityBase> AbilityClass);
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Ability|Data")
	TObjectPtr<UDataTable> SkillDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> DashAbilityClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> TravelerMemory1Class;  // 우클릭 고유스킬

	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> SkillQClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> SkillEClass;

	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> TravelerMemory2Class;  // R 고유스킬
	
	UPROPERTY()
	TMap<EAbilitySlot, TObjectPtr<UAbilityBase>> Abilities;
	
	void RegisterAbility(EAbilitySlot Slot, TSubclassOf<UAbilityBase> AbilityClass);
};
