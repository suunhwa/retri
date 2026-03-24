// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/StatComponent.h"
#include "Player/Components/HealthComponent.h"


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

		case EStatTypes::FireDamage:
			FireDamage = FMath::Max(0.f, FireDamage + Delta);
			newValue = FireDamage;
			break;
	}

	OnStatChanged.Broadcast(Type, newValue);
}

void UStatComponent::LoadStatsForLevel(int32 Level)
{
	if (!StatDataTable) return;

	FName RowName = FName(*FString::Printf(TEXT("Level_%d"), Level));
	FPlayerStatRow* Row = StatDataTable->FindRow<FPlayerStatRow>(RowName, TEXT("LoadStatsForLevel"));
	if (!Row) return;

	CurrentLevel = Level;
	MoveSpeed    = Row->MoveSpeed;
	AttackDamage = Row->AttackDamage;
	AttackSpeed  = Row->AttackSpeed;
	DashCooldown = Row->DashCooldown;
	MaxHP        = Row->MaxHP;

	// HealthComponent MaxHP도 같이 업데이트
	if (UHealthComponent* HC = GetOwner()->FindComponentByClass<UHealthComponent>())
	{
		HC->SetMaxHP(MaxHP, false);
	}

	OnStatChanged.Broadcast(EStatTypes::MaxHP, MaxHP);
}

FPlayerStatInfo UStatComponent::GetStatInfo() const
{
	FPlayerStatInfo Info;
	Info.MoveSpeed    = MoveSpeed;
	Info.AttackDamage = AttackDamage;
	Info.AttackSpeed  = AttackSpeed;
	Info.DashCooldown = DashCooldown;
	Info.MaxHP        = MaxHP;
	Info.FireDamage   = FireDamage;
	Info.CurrentLevel = CurrentLevel;
	return Info;
}


