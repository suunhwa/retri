// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/Skills/PillarOfFlame/FireOrbProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"

AFireOrbProjectile::AFireOrbProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(20.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
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

	if (OrbEffect)
	{
		OrbEffectComp->SetAsset(OrbEffect);
		OrbEffectComp->Activate();
	}
}
