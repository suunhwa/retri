// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/Skills/GravityReversal/GravityReversalAoE.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"

AGravityReversalAoE::AGravityReversalAoE()
{
	PrimaryActorTick.bCanEverTick = false;

	HitVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitVolume"));
	HitVolume->SetCapsuleSize(200.f, 200.f);  // 기본값; BeginPlay에서 HitRadius로 재설정
	HitVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = HitVolume;

	GravityEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GravityEffect"));
	GravityEffect->SetupAttachment(RootComponent);
	GravityEffect->SetAutoActivate(false);
}

void AGravityReversalAoE::BeginPlay()
{
	Super::BeginPlay();
	
	HitVolume->SetCapsuleSize(HitRadius, 200.f);

	// 디버그 범위 표시 (파란색, LiftDuration 동안 유지)
	DrawDebugCylinder(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, 200), HitRadius, 32, FColor::Blue, false, LiftDuration);

	if (GravityEffect)
	{
		GravityEffect->Activate();
	}

	ApplyLift();

	// LiftDuration 후 착지 피해
	GetWorldTimerManager().SetTimer(
		LandingTimerHandle,
		this, &AGravityReversalAoE::ApplyLandingDamage,
		LiftDuration, false
	);

	// Actor 수명: 착지 이후 약간 여유
	SetLifeSpan(LiftDuration + 0.5f);
}

void AGravityReversalAoE::Init(float InAbilityPower, AController* InInstigator)
{
	AbilityPower = InAbilityPower;
	InstigatorController = InInstigator;
}

void AGravityReversalAoE::ApplyLift()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	TArray<AActor*> OverlappingActors;
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), GetActorLocation(), HitRadius,
		ObjectTypes, AEnemyBase::StaticClass(),
		IgnoreActors, OverlappingActors
	);

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor || Actor == GetOwner()) continue;

		ACharacter* EnemyChar = Cast<ACharacter>(Actor);
		if (!EnemyChar) continue;

		// 위로 띄우기
		EnemyChar->LaunchCharacter(FVector(0.f, 0.f, LaunchZVelocity), false, true);

		// 이동 잠금 (XY) — 공중에 있는 동안 제자리 유지
		if (UCharacterMovementComponent* MoveComp = EnemyChar->GetCharacterMovement())
		{
			MoveComp->GravityScale = 0.f;
		}

		LiftedEnemies.Add(Actor);
	}

	if (LiftSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LiftSound, GetActorLocation());
	}
}

void AGravityReversalAoE::ApplyLandingDamage()
{
	const float LandingDamage = AbilityPower * LandingDamageCoeff;

	for (TWeakObjectPtr<AActor> WeakEnemy : LiftedEnemies)
	{
		if (!WeakEnemy.IsValid()) continue;

		// 중력 복구
		if (ACharacter* EnemyChar = Cast<ACharacter>(WeakEnemy.Get()))
		{
			if (UCharacterMovementComponent* MoveComp = EnemyChar->GetCharacterMovement())
			{
				MoveComp->GravityScale = 1.f;
			}
		}

		UGameplayStatics::ApplyDamage(
			WeakEnemy.Get(),
			LandingDamage,
			InstigatorController.Get(),
			this,
			UDamageType::StaticClass()
		);
	}

	LiftedEnemies.Empty();

	if (LandSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LandSound, GetActorLocation());
	}
}
