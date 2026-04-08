// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/Skills/MassCleanse/MassCleanseAoE.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AMassCleanseAoE::AMassCleanseAoE()
{
	PrimaryActorTick.bCanEverTick = false;

	HitVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitVolume"));
	HitVolume->SetCapsuleSize(400.f, 200.f);
	HitVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = HitVolume;

	CleanseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("CleanseEffect"));
	CleanseEffect->SetupAttachment(RootComponent);
	CleanseEffect->SetAutoActivate(false);
}

void AMassCleanseAoE::BeginPlay()
{
	Super::BeginPlay();

	HitVolume->SetCapsuleSize(HitRadius, 200.f);

	if (CleanseEffect)
	{
		CleanseEffect->Activate();
	}
	
	// 디버그: AoE 범위 시각화 
	// DrawDebugSphere(GetWorld(), GetActorLocation(), HitRadius, 32, FColor::White, false, EffectDuration);

	ApplyHit();

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (ImpactCS)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PC->ClientStartCameraShake(ImpactCS);
		}
	}

	GetWorldTimerManager().SetTimer(
		LifeTimerHandle,
		this, &AMassCleanseAoE::FinishEffect,
		EffectDuration, false
	);
}

void AMassCleanseAoE::Init(float InAbilityPower, AController* InInstigator)
{
	AbilityPower = InAbilityPower;
	InstigatorController = InInstigator;
}

void AMassCleanseAoE::ApplyHit()
{
	const float Damage = AbilityPower * DamageCoeff;

	for (TWeakObjectPtr<AActor> WeakEnemy : GetEnemiesInRange())
	{
		if (!WeakEnemy.IsValid()) continue;

		UGameplayStatics::ApplyDamage(
			WeakEnemy.Get(),
			Damage,
			InstigatorController.Get(),
			this,
			UDamageType::StaticClass()
		);
	}
}

void AMassCleanseAoE::FinishEffect()
{
	Destroy();
}

TArray<TWeakObjectPtr<AActor>> AMassCleanseAoE::GetEnemiesInRange() const
{
	TArray<TWeakObjectPtr<AActor>> Result;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), GetActorLocation(), HitRadius,
		ObjectTypes, AEnemyBase::StaticClass(),
		IgnoreActors, OutActors
	);

	for (AActor* Actor : OutActors)
	{
		if (Actor) Result.Add(Actor);
	}

	return Result;
}
