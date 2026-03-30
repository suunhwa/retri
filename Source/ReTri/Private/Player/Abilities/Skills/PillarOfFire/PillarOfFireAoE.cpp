// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/Skills/PillarOfFire/PillarOfFireAoE.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"

APillarOfFireAoE::APillarOfFireAoE()
{
	PrimaryActorTick.bCanEverTick = false;

	HitVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitVolume"));
	HitVolume->SetCapsuleSize(200.f, 200.f);  // 기본값; BeginPlay에서 HitRadius로 재설정
	HitVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = HitVolume;

	FireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(RootComponent);
	FireEffect->SetAutoActivate(false);
}

void APillarOfFireAoE::BeginPlay()
{
	Super::BeginPlay();
	
	HitVolume->SetCapsuleSize(HitRadius, 200.f);

	// 디버그 범위 표시 (주황색, DoTDuration 동안 유지)
	// DrawDebugCylinder(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, 200), HitRadius, 32, FColor::Orange, false, DoTDuration);

	// 이펙트 재생
	if (FireEffect)
	{
		FireEffect->Activate();
	}

	ApplyInitialHit();

	// DoT 설정: DoTDuration 동안 DoTTickInterval마다 틱
	const int32 TotalTicks = FMath::Max(1, FMath::RoundToInt(DoTDuration / DoTTickInterval));
	DoTDamagePerTick = (AbilityPower * DoTDamageCoeff) / TotalTicks;

	GetWorldTimerManager().SetTimer(
		DoTTimerHandle,
		this, &APillarOfFireAoE::ApplyDoTTick,
		DoTTickInterval, true
	);

	// DoT 종료 후 Actor 제거
	GetWorldTimerManager().SetTimer(
		LifeTimerHandle,
		this, &APillarOfFireAoE::FinishDoT,
		DoTDuration, false
	);

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

void APillarOfFireAoE::Init(float InAbilityPower, AController* InInstigator)
{
	AbilityPower = InAbilityPower;
	InstigatorController = InInstigator;
}

void APillarOfFireAoE::ApplyInitialHit()
{
	const float ImmediateDamage = AbilityPower * ImmediateDamageCoeff;

	for (TWeakObjectPtr<AActor> WeakEnemy : GetEnemiesInRange())
	{
		if (!WeakEnemy.IsValid()) continue;

		UGameplayStatics::ApplyDamage(
			WeakEnemy.Get(),
			ImmediateDamage,
			InstigatorController.Get(),
			this,
			UDamageType::StaticClass()
		);

		// 경직: 짧은 시간 동안 이동 비활성화
		if (ACharacter* EnemyChar = Cast<ACharacter>(WeakEnemy.Get()))
		{
			if (UCharacterMovementComponent* MoveComp = EnemyChar->GetCharacterMovement())
			{
				MoveComp->DisableMovement();

				FTimerHandle StaggerHandle;
				TWeakObjectPtr<UCharacterMovementComponent> WeakMoveComp(MoveComp);
				GetWorldTimerManager().SetTimer(StaggerHandle, FTimerDelegate::CreateLambda([WeakMoveComp]()
				{
					if (WeakMoveComp.IsValid())
					{
						WeakMoveComp->SetMovementMode(MOVE_Walking);
					}
				}), StaggerDuration, false);
			}
		}
	}
}

void APillarOfFireAoE::ApplyDoTTick()
{
	for (TWeakObjectPtr<AActor> WeakEnemy : GetEnemiesInRange())
	{
		if (!WeakEnemy.IsValid()) continue;

		UGameplayStatics::ApplyDamage(
			WeakEnemy.Get(),
			DoTDamagePerTick,
			InstigatorController.Get(),
			this,
			UDamageType::StaticClass()
		);
	}
}

void APillarOfFireAoE::FinishDoT()
{
	GetWorldTimerManager().ClearTimer(DoTTimerHandle);
	Destroy();
}

TArray<TWeakObjectPtr<AActor>> APillarOfFireAoE::GetEnemiesInRange() const
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
