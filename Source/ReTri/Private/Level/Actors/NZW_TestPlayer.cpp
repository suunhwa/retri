// Fill out your copyright notice in the Description page of Project Settings.

#include "Level/Actors/NZW_TestPlayer.h"
#include "ReTriGameData.h"
#include "Level/Actors/InteractableBase.h"
#include "ReTriGameData.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ANZW_TestPlayer::ANZW_TestPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GD = CreateDefaultSubobject<UReTriGameData>(TEXT("GameData"));
}

// Called when the game starts or when spawned
void ANZW_TestPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Gold = 100;
	// HP = 100;
	// AttackDamage = 1000;
	// AbilityPower = 200;
	// Armor = 300;
	// AttackSpeed = 100;
	// CriticalChance = 5;
	// CriticalPower = 200;
	// MemoryHaste = 400;
	// MoveSpeed = 600;
	// ElemPower = 100;
	// FireDamage = 50;
	// FrozenDamage = 50;
	// exp = 0;
	
	// GD->DebugStat();
}

// Called every frame
void ANZW_TestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANZW_TestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANZW_TestPlayer::HoverInteractable()
{
	// 감지하고자 하는 오브젝트 타입들을 배열에 담기.
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); // Interaction
	
	FHitResult HitResult;
	bool bHit = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(ObjectTypes, false, HitResult);
	if (bHit && HitResult.GetActor()->Implements<UInteractableInterface>())
	{
		IInteractableInterface::Execute_Hover(HitResult.GetActor());
	}
}

void ANZW_TestPlayer::Interaction()
{
	// F 키를 눌렀을 때 오브젝트 들과 상호작용 할 수 있도록 
	
	// 감지하고자 하는 오브젝트 타입
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); // Interaction
	
	// 제외할 Actors 
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), 42.f, 16, FColor::Red);
	
	// 근처에 Interaction Object가 있는지 감지 
	TArray<AActor*> OutActors;
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 42.f, ObjectTypes, AActor::StaticClass(), IgnoreActors,OutActors);
	
	if (!bHit) return;
	for (AActor* Actor : OutActors)
	{
		AInteractableBase* Interact = Cast<AInteractableBase>(Actor);
		if (Interact)
		{
			IInteractableInterface::Execute_Interact(Interact);
			
			// GD->DebugStat();
		}
	}
}

