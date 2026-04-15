#include "Player/Components/HealthComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ReTriGameInstance.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = AppliedMaxHP;
	OnHPChanged.Broadcast(CurrentHP, AppliedMaxHP);
}

void UHealthComponent::TickComponent(float DeltaTime,
                                     ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UHealthComponent::HandleDamage(float DamageAmount, AController* Instigator)
{
	if (bIsDead || DamageAmount <= 0.0f) return 0.0f;

	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.0f, AppliedMaxHP);
	OnHPChanged.Broadcast(CurrentHP, AppliedMaxHP);

	if (CurrentHP <= 0.0f)
	{
		bIsDead = true;
		OnDeath.Broadcast(Instigator);
	}

	return DamageAmount;
}

void UHealthComponent::Heal(float Amount)
{
	if (bIsDead || Amount <= 0.0f) return;

	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.0f, AppliedMaxHP);
	OnHPChanged.Broadcast(CurrentHP, AppliedMaxHP);

	if (HealEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			HealEffect,
			GetOwner()->GetActorLocation(),
			GetOwner()->GetActorRotation()
		);
	}

	auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI) return;
	GI->PlayerPlayData.SetKillEnemy(Amount);
}

// StatComponent가 계산한 최종 MaxHP를 Health 시스템에 반영
// HealthComponent는 MaxHP의 원본 소유자가 아니라 적용 대상
void UHealthComponent::SetMaxHP(float NewMaxHP, bool bHealToFull)
{
	AppliedMaxHP = FMath::Max(1.f, NewMaxHP);
	CurrentHP = bHealToFull ? AppliedMaxHP : FMath::Min(CurrentHP, AppliedMaxHP);
	OnHPChanged.Broadcast(CurrentHP, AppliedMaxHP);
}
