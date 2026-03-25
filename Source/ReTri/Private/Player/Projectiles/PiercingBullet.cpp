// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectiles/PiercingBullet.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APiercingBullet::APiercingBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(15.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel4);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComp;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(CollisionComp);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MoveComp->SetUpdatedComponent(CollisionComp);
	MoveComp->InitialSpeed = 1800.f;
	MoveComp->MaxSpeed = 1800.f;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->ProjectileGravityScale = 0.f;
	
	// trail
	TrailComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailComp"));
	TrailComp->SetupAttachment(CollisionComp);
	TrailComp->bAutoActivate = false;
	
	InitialLifeSpan = 3.f;
}

// Called when the game starts or when spawned
void APiercingBullet::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APiercingBullet::OnOverlap);
	
	if (TrailEffect)
	{
		TrailComp->SetTemplate(TrailEffect);
		TrailComp->Activate();
	}
}

// Called every frame
void APiercingBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APiercingBullet::OnOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;
	
	// 이미 맞은 액터 스킵
	if (HitActors.Contains(OtherActor)) return;
	
	HitActors.Add(OtherActor);
	PierceCount++;
	
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	
	// effects
	// niagara
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
	}
	
	// particle
	if (ParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, GetActorLocation());
	}
	
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
	
	// 최대 관통 수 초과 시 소멸
	if (PierceCount >= MaxPierceCount)
	{
		Destroy();
	}
}
