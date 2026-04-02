// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/Skills/MassCleanse/MassCleanseAoE.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AMassCleanseAoE::AMassCleanseAoE()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	HitVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitVolume"));
	HitVolume->SetCapsuleSize(200.f, 200.f);  // 기본값; BeginPlay에서 HitRadius로 재설정
	HitVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = HitVolume;

	CleanseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireEffect"));
	CleanseEffect->SetupAttachment(RootComponent);
	CleanseEffect->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AMassCleanseAoE::BeginPlay()
{
	Super::BeginPlay();
	
	HitVolume->SetCapsuleSize(HitRadius, 200.f);

	// 디버그 범위 표시 (주황색, DoTDuration 동안 유지)
	// DrawDebugCylinder(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, 200), HitRadius, 32, FColor::Orange, false, DoTDuration);

	// 이펙트 재생
	if (CleanseEffect)
	{
		CleanseEffect->Activate();
	}

	ApplyInitialHit();

	// DoT 설정: DoTDuration 동안 DoTTickInterval마다 틱
	const int32 TotalTicks = FMath::Max(1, FMath::RoundToInt(DoTDuration / DoTTickInterval));
	DoTDamagePerTick = (AbilityPower * DoTDamageCoeff) / TotalTicks;

	GetWorldTimerManager().SetTimer(
		DoTTimerHandle,
		this, &AMassCleanseAoE::ApplyDoTTick,
		DoTTickInterval, true
	);

	// DoT 종료 후 Actor 제거
	GetWorldTimerManager().SetTimer(
		LifeTimerHandle,
		this, &AMassCleanseAoE::FinishDoT,
		DoTDuration, false
	);

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

void AMassCleanseAoE::Init(float InAbilityPower, AController* InInstigator)
{
	AbilityPower = InAbilityPower;
	InstigatorController = InInstigator;
}

void AMassCleanseAoE::ApplyInitialHit()
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

void AMassCleanseAoE::ApplyDoTTick()
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

void AMassCleanseAoE::FinishDoT()
{
	GetWorldTimerManager().ClearTimer(DoTTimerHandle);
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




