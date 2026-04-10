// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectiles/Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

	TrailEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffectComp"));
	TrailEffectComp->SetupAttachment(collisionComp);
	TrailEffectComp->SetAutoActivate(false);

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

	if (TrailEffectComp && TrailEffect && !bIsEnhanced)
	{
		TrailEffectComp->SetAsset(TrailEffect);
		TrailEffectComp->Activate();
	}

	USoundBase* ActiveFireSound = bIsEnhanced && EnhancedFireSound ? EnhancedFireSound.Get() : FireSound.Get();
	if (ActiveFireSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ActiveFireSound, GetActorLocation());
}

void ABullet::SetEnhanced()
{
	bIsEnhanced = true;

	if (TrailEffectComp && EnhancedTrailEffect)
	{
		TrailEffectComp->SetAsset(EnhancedTrailEffect);
		TrailEffectComp->Activate();
	}
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor || OtherActor == GetOwner()) return;

	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	UNiagaraSystem* ActiveHitEffect = bIsEnhanced && EnhancedHitEffect ? EnhancedHitEffect.Get() : HitEffect.Get();
	USoundBase*     ActiveHitSound  = bIsEnhanced && EnhancedHitSound  ? EnhancedHitSound.Get()  : HitSound.Get();

	if (ActiveHitEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ActiveHitEffect, GetActorLocation());

	if (ActiveHitSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ActiveHitSound, GetActorLocation());
	
	
	Destroy();
}
