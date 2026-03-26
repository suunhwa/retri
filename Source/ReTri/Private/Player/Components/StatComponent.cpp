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

float UStatComponent::CalcStat(float Base, float Growth, int32 Level)
{
	return Base + Growth * (Level - 1);
}

void UStatComponent::ApplyStatModifier(EStatTypes Type, float Delta)
{
	float newValue = 0.f;

	switch (Type)
	{
	case EStatTypes::MaxHP:
		MaxHP = FMath::Max(1.f, MaxHP + Delta);
		newValue = MaxHP;
		break;

	case EStatTypes::AttackDamage:
		AttackDamage = FMath::Max(0.f, AttackDamage + Delta);
		newValue = AttackDamage;
		break;

	case EStatTypes::AbilityPower:
		AbilityPower = FMath::Max(0.f, AbilityPower + Delta);
		newValue = AbilityPower;
		break;

	case EStatTypes::AttackSpeed:
		AttackSpeed = FMath::Max(0.1f, AttackSpeed + Delta);
		newValue = AttackSpeed;
		break;

	case EStatTypes::CritChance:
		CritChance = FMath::Clamp(CritChance + Delta, 0.f, 100.f);
		newValue = CritChance;
		break;
		
	case EStatTypes::MoveSpeed:
		MoveSpeed = FMath::Max(0.f, MoveSpeed + Delta);
		newValue = MoveSpeed;
		break;
		
	case EStatTypes::Defense:
		Defense = FMath::Max(0.f, Defense + Delta);
		newValue = Defense;
		break;
		
	case EStatTypes::MemoryHaste:
		MemoryHaste = FMath::Max(0.f, MemoryHaste + Delta);
		newValue = MemoryHaste;
		break;
		
	case EStatTypes::DashCooldown:
		DashCooldown = FMath::Max(0.1f, DashCooldown + Delta);
		newValue = DashCooldown;
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
	if (!StatDataTable || !ExpTable) return;

	FPlayerStatRow* Base = StatDataTable->FindRow<FPlayerStatRow>(TEXT("Player"), TEXT("LoadStatsForLevel"));
	if (!Base) return;

	CurrentLevel = Level;
	MaxHP = CalcStat(Base->BaseHP, Base->HPGrowth, Level);
	AttackDamage = CalcStat(Base->BaseAD, Base->ADGrowth, Level);
	AbilityPower = CalcStat(Base->BaseAP, Base->APGrowth, Level);
	AttackSpeed = CalcStat(Base->BaseAS, Base->ASGrowth, Level);
	CritChance = CalcStat(Base->BaseCrit, Base->CritGrowth, Level);
	MoveSpeed = CalcStat(Base->BaseMS, Base->MSGrowth, Level);
	Defense = CalcStat(Base->BaseDefense, Base->DefenseGrowth, Level);
	MemoryHaste = CalcStat(Base->BaseHaste, Base->HasteGrowth, Level);
	DashCooldown = CalcStat(Base->BaseDashCooldown, Base->DashCooldownGrowth, Level);
	FireDamage = CalcStat(Base->BaseFire, Base->FireGrowth, Level);

	// HealthComponent MaxHP도 같이 업데이트
	if (UHealthComponent* HC = GetOwner()->FindComponentByClass<UHealthComponent>())
	{
		HC->SetMaxHP(MaxHP, false);
	}

	OnStatChanged.Broadcast(EStatTypes::MaxHP, MaxHP);
}


int32 UStatComponent::GetRequiredExpForLevel(int32 Level) const
{
	if (!ExpTable) return -1;

	FName RowName = FName(*FString::Printf(TEXT("Level_%d"), Level));
	FExpTableRow* Row = ExpTable->FindRow<FExpTableRow>(RowName, TEXT("GetRequiredExpForLevel"));
	return Row ? Row->RequiredExp : -1;
}

FPlayerStatInfo UStatComponent::GetStatInfo() const
{
	FPlayerStatInfo Info;
	Info.MaxHP = MaxHP;
	Info.AttackDamage = AttackDamage;
	Info.AbilityPower = AbilityPower;
	Info.AttackSpeed = AttackSpeed;
	Info.CritChance = CritChance;
	Info.MoveSpeed = MoveSpeed;
	Info.Defense = Defense;
	Info.MemoryHaste = MemoryHaste;
	Info.DashCooldown = DashCooldown;
	Info.FireDamage = FireDamage;
	Info.CurrentLevel = CurrentLevel;
	return Info;
}
