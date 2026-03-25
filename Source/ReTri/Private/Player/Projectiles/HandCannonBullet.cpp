// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectiles/HandCannonBullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AHandCannonBullet::AHandCannonBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetSphereRadius(25.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel6);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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
	
	// trail
	TrailComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailComp"));
	TrailComp->SetupAttachment(CollisionComp);
	TrailComp->bAutoActivate = false;
	
	InitialLifeSpan = 3.f;
}

// Called when the game starts or when spawned
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

}

// Called every frame
void AHandCannonBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	
	// particle
	if (ParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, GetActorLocation());
	}
	
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	}
	
	Destroy();
}

