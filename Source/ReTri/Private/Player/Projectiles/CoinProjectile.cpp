#include "Player/Projectiles/CoinProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"

ACoinProjectile::ACoinProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(30.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel5);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Overlap);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComp->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComp;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(CollisionComp);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MoveComp->SetUpdatedComponent(CollisionComp);
	MoveComp->InitialSpeed = 1400.f;
	MoveComp->MaxSpeed = 1400.f;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;
}

void ACoinProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACoinProjectile::OnOverlap);

	if (FlightEffect)
	{
		FlightEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			FlightEffect,
			BodyMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	if (FlightSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlightSound, GetActorLocation());
	}
}

void ACoinProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasExploded)
	{
		BodyMesh->AddLocalRotation(FRotator(0.f, SpinRate * DeltaTime, 0.f));
	}
}

void ACoinProjectile::Init(float InDamage, AController* InInstigator, FOnCoinExploded InCallback)
{
	Damage = InDamage;
	InstigatorController = InInstigator;
	OnCoinExploded = InCallback;
}

void ACoinProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp,
                                AActor* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex,
                                bool bFromSweep,
                                const FHitResult& SweepResult)
{
	if (bHasExploded) return;
	if (!OtherActor || OtherActor == GetOwner()) return;

	// 적에만 반응
	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
	if (!Enemy) return;

	bHasExploded = true;
	MoveComp->StopMovementImmediately();

	Explode(GetActorLocation());
}

void ACoinProjectile::Explode(FVector ExplosionLocation)
{
	if (FlightEffectComp)
	{
		FlightEffectComp->DeactivateImmediate();
	}

	// VFX
	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionEffect,
			ExplosionLocation,
			FRotator::ZeroRotator,
			FVector(1.f),
			true,
			true,
			ENCPoolMethod::None,
			true
		);
	}

	// SFX
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, ExplosionLocation);
	}

	if (ExplosionCS)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PC->ClientStartCameraShake(ExplosionCS);
		}
	}

	// AoE 범위 내 적 탐색
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel7));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	TArray<AActor*> HitActors;
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		ExplosionLocation,
		ExplosionRadius,
		ObjectTypes,
		AEnemyBase::StaticClass(),
		IgnoreActors,
		HitActors
	);

	for (AActor* Actor : HitActors)
	{
		if (!Actor) continue;

		// 빛 피해 적용
		UGameplayStatics::ApplyDamage(
			Actor,
			Damage,
			InstigatorController.Get(),
			this,
			UDamageType::StaticClass()
		);

		// 1초 기절: 이동 비활성화
		if (ACharacter* EnemyChar = Cast<ACharacter>(Actor))
		{
			if (UCharacterMovementComponent* EnemyMoveComp = EnemyChar->GetCharacterMovement())
			{
				EnemyMoveComp->DisableMovement();
			}

			// AI 일시 정지
			if (AController* EnemyController = EnemyChar->GetController())
			{
				EnemyController->StopMovement();
			}

			// 기절 해제 타이머
			FTimerHandle StunTimer;
			TWeakObjectPtr<AActor> WeakActor(Actor);
			GetWorld()->GetTimerManager().SetTimer(StunTimer,
			                                       [this, WeakActor]()
			                                       {
				                                       UnstunEnemy(WeakActor);
			                                       },
			                                       StunDuration,
			                                       false);
		}
	}

	// 폭발 완료 → 스킬에 50/50 판정 요청
	OnCoinExploded.ExecuteIfBound();

	Destroy();
}

void ACoinProjectile::UnstunEnemy(TWeakObjectPtr<AActor> WeakEnemy)
{
	if (!WeakEnemy.IsValid()) return;

	ACharacter* EnemyChar = Cast<ACharacter>(WeakEnemy.Get());
	if (!EnemyChar) return;

	if (AEnemyBase* Enemy = Cast<AEnemyBase>(EnemyChar))
	{
		if (Enemy->bIsDead) return;
	}

	if (UCharacterMovementComponent* EnemyMoveComp = EnemyChar->GetCharacterMovement())
	{
		EnemyMoveComp->SetMovementMode(MOVE_Walking);
	}
}
