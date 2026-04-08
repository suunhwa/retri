// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/Skills/PillarOfFlame/PillarOfFlameAoE.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"

APillarOfFlameAoE::APillarOfFlameAoE()
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

void APillarOfFlameAoE::BeginPlay()
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
		this, &APillarOfFlameAoE::ApplyDoTTick,
		DoTTickInterval, true
	);

	// DoT 종료 후 Actor 제거
	GetWorldTimerManager().SetTimer(
		LifeTimerHandle,
		this, &APillarOfFlameAoE::FinishDoT,
		DoTDuration, false
	);

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
}

void APillarOfFlameAoE::Init(float InAbilityPower, AController* InInstigator)
{
	AbilityPower = InAbilityPower;
	InstigatorController = InInstigator;
}

void APillarOfFlameAoE::ApplyInitialHit()
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

		// 불타는 이펙트 붙이기
		if (BurnEffect)
		{
			UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAttached(
				BurnEffect,
				Cast<ACharacter>(WeakEnemy.Get()) ? Cast<ACharacter>(WeakEnemy.Get())->GetMesh() : WeakEnemy.Get()->GetRootComponent(),
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				false
			);
			if (NC) BurnEffectComps.Add(NC);
		}

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

void APillarOfFlameAoE::ApplyDoTTick()
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

void APillarOfFlameAoE::FinishDoT()
{
	// 불타는 이펙트 제거
	for (TWeakObjectPtr<UNiagaraComponent> WeakNC : BurnEffectComps)
	{
		if (WeakNC.IsValid())
		{
			WeakNC->DeactivateImmediate();
		}
	}
	BurnEffectComps.Empty();

	GetWorldTimerManager().ClearTimer(DoTTimerHandle);

	// FireEffect 자연 페이드아웃 후 액터 제거
	if (FireEffect)
	{
		FireEffect->Deactivate();
	}

	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this, &APillarOfFlameAoE::DestroyAfterFade,
		EffectFadeOutDelay, false
	);
}

void APillarOfFlameAoE::DestroyAfterFade()
{
	Destroy();
}

TArray<TWeakObjectPtr<AActor>> APillarOfFlameAoE::GetEnemiesInRange() const
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
