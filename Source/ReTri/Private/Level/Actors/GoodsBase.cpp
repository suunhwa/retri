// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoodsBase::AGoodsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("BoxComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereComp->SetSphereRadius(30.f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);
}

// Called when the game starts or when spawned
void AGoodsBase::BeginPlay()
{
	Super::BeginPlay();
	
	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);	
}

// Called every frame
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
		
		// Yaw와 Roll 축으로만 회전 값 생성
		FRotator DeltaRot = FRotator(0.f, YawRotSpeed * DeltaTime, RollRotSpeed * DeltaTime);
		// 현재 회전에 이만큼을 '더해라' (Local 기준)
		AddActorLocalRotation(DeltaRot);
	}
}

