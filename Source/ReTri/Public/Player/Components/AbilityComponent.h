// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UAbilityBase;

UENUM(BlueprintType)
enum class EAbilitySlot : uint8
{
	Dash,
	SkillQ,
	SkillE,
	SkillR,
};

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
	// 
	UFUNCTION(BlueprintCallable, Category="Ability")
	bool TryActivate(EAbilitySlot Slot);
	
	// UI에서 슬롯별 스킬 오브젝트 접근
	UFUNCTION(BlueprintCallable, Category="Ability")
	UAbilityBase* GetAbility(EAbilitySlot Slot) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> DashAbilityClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> SkillQClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> SkillEClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Slot")
	TSubclassOf<UAbilityBase> SkillRClass;
	
	UPROPERTY()
	TMap<EAbilitySlot, TObjectPtr<UAbilityBase>> Abilities;
	
	void RegisterAbility(EAbilitySlot Slot, TSubclassOf<UAbilityBase> AbilityClass);
	
};
