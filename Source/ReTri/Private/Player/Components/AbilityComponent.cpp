// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/AbilityComponent.h"

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
	
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	return Ability->TryActivate(OwnerChar);
}

UAbilityBase* UAbilityComponent::GetAbility(EAbilitySlot Slot) const
{
	const TObjectPtr<UAbilityBase>* Found = Abilities.Find(Slot);
	return Found ? Found->Get() : nullptr;
}

void UAbilityComponent::RegisterAbility(EAbilitySlot Slot, TSubclassOf<UAbilityBase> AbilityClass)
{
	if (!AbilityClass) return;
	
	UAbilityBase* Instance = NewObject<UAbilityBase>(GetOwner(), AbilityClass);
	Abilities.Add(Slot, Instance);
}
