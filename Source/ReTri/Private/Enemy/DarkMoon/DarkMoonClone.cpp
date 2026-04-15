// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values

#include "Enemy/DarkMoon/DarkMoonClone.h"
#include "Components/BoxComponent.h"

ADarkMoonClone::ADarkMoonClone()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	
	if (GetMesh())
	{
		SwordCollision->SetupAttachment(GetMesh(), TEXT("SwordSocket"));
		SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SwordCollision->SetRelativeLocation(FVector(0.000000,65.000000,0.000000));
		SwordCollision->SetBoxExtent(FVector(14.175652,58.968844,10.359240));
		SwordCollision->SetGenerateOverlapEvents(true);
	}
}

void ADarkMoonClone::BeginPlay()
{
	Super::BeginPlay();
	
	SwordCollision->AttachToComponent(GetMesh(), 
		FAttachmentTransformRules::KeepRelativeTransform, 
		TEXT("SwordSocket"));
	
	SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &ADarkMoonClone::OnSwordOverlap);
}

void ADarkMoonClone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADarkMoonClone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADarkMoonClone::OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnAttackOverlap(OtherActor);
}

void ADarkMoonClone::OnSwordCollision()
{
	if (SwordCollision)
	{	
		SwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ADarkMoonClone::OffSwordCollision()
{
	if (SwordCollision)
	{
		// ZW 콜리젼
		bHasHitTarget = false;
		SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}