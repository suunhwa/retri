// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/AbilityComponent.h"

#include "ReTriGameInstance.h"
#include "GameFramework/Character.h"
#include "Player/Abilities/AbilityBase.h"


// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());

	// 레벨 이동 시 저장된 슬롯 매핑 복원
	if (GI)
	{
		if (GI->bHasSavedStats && GI->SavedSkillSlots.Num() > 0)
		{
			for (auto& Pair : GI->SavedSkillSlots)
			{
				EAbilitySlot Slot = static_cast<EAbilitySlot>(Pair.Key);
				if (Pair.Value)
					RegisterAbility(Slot, Pair.Value);
			}
			return; // 복원 완료, 아래 기본 등록 스킵
		}
	}
	RegisterAbility(EAbilitySlot::Dash, DashAbilityClass);
	RegisterAbility(EAbilitySlot::TravelerMemory1, TravelerMemory1Class);
	RegisterAbility(EAbilitySlot::SkillQ, SkillQClass);
	RegisterAbility(EAbilitySlot::SkillE, SkillEClass);
	RegisterAbility(EAbilitySlot::TravelerMemory2, TravelerMemory2Class);
	
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
									  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAbilityComponent::TryActivate(EAbilitySlot Slot)
{
	UAbilityBase* Ability = GetAbility(Slot);
	if (!Ability) return false;
	/*if (!Ability)
	{
		UE_LOG(LogTemp, Error, TEXT("TryActivate FAILED: Slot %d not registered"), (int32)Slot);
		return false;
	}*/
	
	// UE_LOG(LogTemp, Warning, TEXT("RegisterAbility SUCCESS: Slot %d"), (int32)Slot);
	
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	return Ability->TryActivate(OwnerChar);
}

UAbilityBase* UAbilityComponent::GetAbility(EAbilitySlot Slot) const
{
	const TObjectPtr<UAbilityBase>* Found = Abilities.Find(Slot);
	return Found ? Found->Get() : nullptr;
}

void UAbilityComponent::SetSkill(EAbilitySlot Slot, TSubclassOf<UAbilityBase> AbilityClass)
{
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	// 전체 슬롯 매핑 GI에 저장
	if (GI)
	{
		GI->SavedSkillSlots.Add(static_cast<uint8>(Slot), AbilityClass);
	}
	RegisterAbility(Slot, AbilityClass);
	OnSkillSlotChanged.Broadcast(Slot);
}

bool UAbilityComponent::EquipAcquiredSkill(TSubclassOf<UAbilityBase> AbilityClass)
{
	// Q 슬롯 먼저, 비어있으면 장착
	if (!GetAbility(EAbilitySlot::SkillQ))
	{
		SetSkill(EAbilitySlot::SkillQ, AbilityClass);
		return true;
	}
	// E 슬롯 비어있으면 장착
	if (!GetAbility(EAbilitySlot::SkillE))
	{
		SetSkill(EAbilitySlot::SkillE, AbilityClass);
		return true;
	}
	// 둘 다 차있으면 실패 (상점에서 슬롯 선택 UI 필요)
	return false;
}

void UAbilityComponent::RegisterAbility(EAbilitySlot Slot, TSubclassOf<UAbilityBase> AbilityClass)
{
	if (!AbilityClass)
	{
		Abilities.Remove(Slot);
		return;
	}

	UAbilityBase* Instance = NewObject<UAbilityBase>(GetOwner(), AbilityClass);

	// 데이터 테이블에서 스킬 데이터 초기화
	if (SkillDataTable && Instance->DataTableRowName != NAME_None)
	{
		Instance->InitFromDataTable(SkillDataTable, Instance->DataTableRowName);
	}

	Abilities.Add(Slot, Instance);
}
