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
	
	if (TargetPlayer)
	{
		FVector CurLoc = GetActorLocation();
        
		// 플레이어 발밑이 아니라 가슴팍 정도로 빨려오게 위치 보정
		FVector TargetLoc = TargetPlayer->GetActorLocation() + FVector(0, 0, 50.0f);
        
		// VInterpTo: 현재 위치에서 타겟 위치로 부드럽게 이동. 속도 InterpSpeed로 조정
		FVector NewLoc = FMath::VInterpTo(CurLoc, TargetLoc, DeltaTime, Speed);
        
		SetActorLocation(NewLoc);
	}
}

void AGoodsBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	

}

