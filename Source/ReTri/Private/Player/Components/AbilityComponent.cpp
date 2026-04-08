// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/AbilityComponent.h"

#include "ReTriGameInstance.h"
#include "GameFramework/Character.h"
#include "Item/ItemBase.h"
#include "Player/Abilities/AbilityBase.h"
#include "Player/Data/PlayerSkillData.h"


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
			
			// 저장 안 된 슬롯은 기본값으로 채움
			if (!GetAbility(EAbilitySlot::Dash)) RegisterAbility(EAbilitySlot::Dash, DashAbilityClass);
			if (!GetAbility(EAbilitySlot::TravelerMemory1)) RegisterAbility(EAbilitySlot::TravelerMemory1, TravelerMemory1Class);
			if (!GetAbility(EAbilitySlot::TravelerMemory2)) RegisterAbility(EAbilitySlot::TravelerMemory2, TravelerMemory2Class);
			return;
		}
	}
	// 초기 장착: 대시 + 핸드캐논(RMB) + 빠른손(R) 만 장착. Q/E 슬롯은 빈 상태로 시작
	RegisterAbility(EAbilitySlot::Dash, DashAbilityClass);
	RegisterAbility(EAbilitySlot::TravelerMemory1, TravelerMemory1Class);
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
	if (!AbilityClass) return false;

	// 이미 같은 스킬이 장착되어 있으면 중복 장착 거부
	for (auto& Pair : Abilities)
	{
		if (Pair.Value && Pair.Value->GetClass() == AbilityClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AbilityComponent] 중복 스킬 장착 거부: %s"), *AbilityClass->GetName());
			return false;
		}
	}

	// 교체 가능한 4개 슬롯 중 빈 슬롯에 장착 (Q→E→RMB→R 순)
	// 슬롯 간 스왑으로 배치가 바뀔 수 있으므로 고정 슬롯(Dash)만 제외하고 전부 확인
	constexpr EAbilitySlot Swappable[] = {
		EAbilitySlot::SkillQ,
		EAbilitySlot::SkillE,
		EAbilitySlot::TravelerMemory1,
		EAbilitySlot::TravelerMemory2,
	};

	for (EAbilitySlot Slot : Swappable)
	{
		if (!GetAbility(Slot))
		{
			SetSkill(Slot, AbilityClass);
			return true;
		}
	}
	return false;
}


void UAbilityComponent::DropSkill(EAbilitySlot Slot, FVector SpawnLocation)
{
	UAbilityBase* Ability = GetAbility(Slot);
	if (!Ability) return;

	if (DropItemClass)
	{
		UWorld* World = GetWorld();
		AItemBase* Item = World->SpawnActor<AItemBase>(DropItemClass, SpawnLocation, FRotator::ZeroRotator);
		if (Item)
		{
			Item->AbilityClass = Ability->GetClass();

			// 데이터 테이블에서 스킬 정보 조회해 아이템 UI 초기화
			if (SkillDataTable && Ability->DataTableRowName != NAME_None)
			{
				if (FPlayerSkillData* Row = SkillDataTable->FindRow<FPlayerSkillData>(
					Ability->DataTableRowName, TEXT("DropSkill")))
				{
					Item->DataInit(*Row);
				}
			}
		}
	}

	SetSkill(Slot, nullptr);
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
