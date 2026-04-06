// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectiles/Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetSphereRadius(15.f);
	collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionComp->SetCollisionObjectType(ECC_GameTraceChannel5);
	collisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	collisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); // dark moon
	collisionComp->SetGenerateOverlapEvents(true);
	RootComponent = collisionComp;

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	bodyMesh->SetupAttachment(collisionComp);

	moveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	moveComp->SetUpdatedComponent(collisionComp);
	moveComp->InitialSpeed = 1500.f;
	moveComp->MaxSpeed = 1500.f;
	moveComp->bRotationFollowsVelocity = true;
	moveComp->ProjectileGravityScale = 0.f; // 탑다운이라 중력 제거

	InitialLifeSpan = 3.f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
}

void ABullet::SetEnhanced()
{
	bIsEnhanced = true;
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor || OtherActor == GetOwner()) return;

	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	UParticleSystem* HitFX    = bIsEnhanced && EnhancedImpactEffect ? EnhancedImpactEffect : ImpactEffect;
	USoundBase*      HitSound = bIsEnhanced && EnhancedImpactSound  ? EnhancedImpactSound  : ImpactSound;

	if (HitFX)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, GetActorLocation());

	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	
	
	Destroy();
}
