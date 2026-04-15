#include "Player/Projectiles/HandCannonBullet.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"

AHandCannonBullet::AHandCannonBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(25.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel5);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); // dark moon
	CollisionComp->SetGenerateOverlapEvents(true);
	RootComponent = CollisionComp;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(CollisionComp);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MoveComp->SetUpdatedComponent(CollisionComp);
	MoveComp->InitialSpeed = 1000.f;
	MoveComp->MaxSpeed = 1000.f;

	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale = 0.f;
	MoveComp->bSweepCollision = false;

	// trail
	TrailComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailComp"));
	TrailComp->SetupAttachment(CollisionComp);
	TrailComp->bAutoActivate = false;

	InitialLifeSpan = 3.f;
}

void AHandCannonBullet::BeginPlay()
{
	Super::BeginPlay();

	SpawnLocation = GetActorLocation();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AHandCannonBullet::OnOverlap);

	if (TrailEffect)
	{
		TrailComp->SetTemplate(TrailEffect);
		TrailComp->Activate();
	}

	if (NiagaraTrailEffect)
	{
		NiagaraTrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraTrailEffect,
			CollisionComp,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
}

void AHandCannonBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHandCannonBullet::SetDamage(float InNearDamage, float InFarDamage)
{
	NearDamage = InNearDamage;
	FarDamage = InFarDamage;
}

void AHandCannonBullet::OnOverlap(UPrimitiveComponent* OverlappedComp,
                                  AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp,
                                  int32 OtherBodyIndex,
                                  bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	// 발사 위치 기준 거리로 근/원거리 판정
	float distance = FVector::Dist(SpawnLocation, GetActorLocation());
	bool bIsNear = (distance <= NearRange);

	float Damage = bIsNear ? NearDamage : FarDamage;
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	// 근거리 넉백
	if (bIsNear)
	{
		if (ACharacter* HitChar = Cast<ACharacter>(OtherActor))
		{
			FVector KnockDir = OtherActor->GetActorLocation() - SpawnLocation;
			KnockDir.Z = 0.f;
			KnockDir.Normalize();
			HitChar->LaunchCharacter(KnockDir * KnockBackForce, true, false);
		}
	}

	// effects
	// niagara
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
	}

	// 히트 스파크 — 맞은 표면 위치 기준
	const FVector HitPoint = SweepResult.ImpactPoint.IsZero()
		                         ? OtherActor->GetActorLocation()
		                         : FVector(SweepResult.ImpactPoint);

	if (HitSparkEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitSparkEffect, HitPoint);
	}

	if (HitSparkParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitSparkParticle, HitPoint);
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	}

	Destroy();
}
