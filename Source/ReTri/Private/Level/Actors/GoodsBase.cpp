

#include "Level/Actors/GoodsBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AGoodsBase::AGoodsBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BoxComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereComp->SetSphereRadius(30.f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);
}

void AGoodsBase::BeginPlay()
{
	Super::BeginPlay();
	
	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);	
}

void AGoodsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	MoveAndRotate(DeltaTime);
}

void AGoodsBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
}

void AGoodsBase::MoveAndRotate(float DeltaTime)
{
	if (TargetPlayer)
	{
		FVector CurLoc = GetActorLocation();
		FVector TargetLoc = TargetPlayer->GetActorLocation() + FVector(0, 0, 50.0f);
		FVector NewLoc = FMath::VInterpTo(CurLoc, TargetLoc, DeltaTime, Speed);
		SetActorLocation(NewLoc);
		
		FRotator DeltaRot = FRotator(0.f, YawRotSpeed * DeltaTime, RollRotSpeed * DeltaTime);
		AddActorLocalRotation(DeltaRot);
	}
}

