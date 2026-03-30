// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/LootBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALootBase::ALootBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BoxComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCapsuleHalfHeight(200.f);
	CapsuleComp->SetCapsuleRadius(150.f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);
	//MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	//MeshComp->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
}

// Called when the game starts or when spawned
void ALootBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ALootBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) return;
	if (bIsBroken == true) return;
	
	OtherActor->Destroy();
	
	// 공격 Actor와 오버랩 되었을 떄
	HP -= 1;
	
	if (HP <= 0)
	{
		bIsBroken = true;
		Break();
	}
}

void ALootBase::Break()
{
	// 깨졌을 때 이부분은 각각 자식들이 구현
	Destroy();
}

