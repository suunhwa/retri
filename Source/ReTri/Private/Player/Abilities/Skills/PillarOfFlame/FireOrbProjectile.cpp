// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/Skills/PillarOfFlame/FireOrbProjectile.h"
#include "Player/Abilities/Skills/PillarOfFlame/PillarOfFlameAoE.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AFireOrbProjectile::AFireOrbProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(20.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	RootComponent = CollisionComp;

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MoveComp->InitialSpeed = 1200.f;
	MoveComp->MaxSpeed = 1200.f;
	MoveComp->ProjectileGravityScale = 0.f;
	MoveComp->bRotationFollowsVelocity = true;

	OrbEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OrbEffectComp"));
	OrbEffectComp->SetupAttachment(RootComponent);
	OrbEffectComp->SetAutoActivate(false);

	InitialLifeSpan = 5.f;
}

void AFireOrbProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentHit.AddDynamic(this, &AFireOrbProjectile::OnHit);

	if (OrbEffect)
	{
		OrbEffectComp->SetAsset(OrbEffect);
		OrbEffectComp->Activate();
	}
}

void AFireOrbProjectile::Init(float InAbilityPower, AController* InInstigator, TSubclassOf<APillarOfFlameAoE> InAoEClass)
{
	AbilityPower = InAbilityPower;
	InstigatorController = InInstigator;
	AoEClass = InAoEClass;
}

void AFireOrbProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SpawnPillar();
}

void AFireOrbProjectile::SpawnPillar()
{
	if (!AoEClass) return;

	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
	}

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetActorLocation());

	APillarOfFlameAoE* Pillar = GetWorld()->SpawnActorDeferred<APillarOfFlameAoE>(
		AoEClass, SpawnTransform,
		GetOwner(), Cast<APawn>(GetOwner()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (Pillar)
	{
		Pillar->Init(AbilityPower, InstigatorController.Get());
		UGameplayStatics::FinishSpawningActor(Pillar, SpawnTransform);
	}

	Destroy();
}
