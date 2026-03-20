// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/StatComponent.h"


// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStatComponent::ApplyStatModifier(EStatTypes Type, float Delta)
{
	float newValue = 0.f;
	
	switch (Type)
	{
		case EStatTypes::MoveSpeed:
     		MoveSpeed = FMath::Max(0.f, MoveSpeed + Delta);
     		newValue = MoveSpeed;
     		break;
		case EStatTypes::AttackDamage:
			AttackDamage = FMath::Max(0.f, AttackDamage + Delta);
			newValue = AttackDamage;
			break;
		
		case EStatTypes::AttackSpeed:
			AttackSpeed = FMath::Max(0.1f, AttackSpeed + Delta);
			newValue = AttackSpeed;
			break;
		
		case EStatTypes::DashCooldown:
			DashCooldown = FMath::Max(0.1f, DashCooldown + Delta);
			newValue = DashCooldown;
			break;
		
		case EStatTypes::MaxHP:
			MaxHP = FMath::Max(1.0f, MaxHP + Delta);
			newValue = MaxHP;
			break;
	}
	
	OnStatChanged.Broadcast(Type, newValue);
}


