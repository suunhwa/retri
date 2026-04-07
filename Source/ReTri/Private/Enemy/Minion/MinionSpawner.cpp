// Fill out your copyright notice in the Description page of Project Settings.


// Sets default values

#include "Enemy/Minion/MinionSpawner.h"

#include "MapSubSystem.h"
#include "Components/BoxComponent.h"
#include "Enemy/Minion/Minion.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "ReTri/ReTri.h"

AMinionSpawner::AMinionSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	
	BoxComp->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AMinionSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AMinionSpawner::OnOverlapBegin);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), SpawnPointTag, SpawnPoints);
}

// Called every frame
void AMinionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinionSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// === Level Clear ===
	if (auto MapSub = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UMapSubSystem>())
	{
		if (MapSub->CurMapDatas[MapSub->CurMapIndex].bIsCleared)
		{
			MapSub->LevelClear();
			return;
		}
		
		if (OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			CreateMinion();
			BoxComp->OnComponentBeginOverlap.RemoveDynamic(this,&AMinionSpawner::OnOverlapBegin);
		}
	}
}

void AMinionSpawner::CreateMinion()
{
	// 스폰 위치 (target point 액터)
	
	for (int i=0; i < MaxlMinionCount; i++)
	{
		FVector Pos = SpawnPoints[i]->GetActorLocation();
		if (SpawnPoints[i]->Tags.Contains("Dawn"))
		{
			auto mini = GetWorld()->SpawnActor<AMinion>(DawnFactory, Pos, FRotator::ZeroRotator);
			mini->OnMinionDieDelegate.AddDynamic(this, &AMinionSpawner::AddMinionDeathCount);
			mini->AmountExp = FMath::RandRange(5, 20);
		}
		else if (SpawnPoints[i]->Tags.Contains("Dusk"))
		{
			auto mini = GetWorld()->SpawnActor<AMinion>(DuskFactory, Pos, FRotator::ZeroRotator);
			mini->OnMinionDieDelegate.AddDynamic(this, &AMinionSpawner::AddMinionDeathCount);
			mini->AmountExp = FMath::RandRange(30, 50);
		}
	}
}

void AMinionSpawner::AddMinionDeathCount()
{
	CurrentDeathCount++;
	
	if (CurrentDeathCount >= MaxlMinionCount)
	{
		SCREENLOG("전멸.");
		
		if (auto GI = UGameplayStatics::GetGameInstance(GetWorld()))
		{
			if (auto MapSub = GI->GetSubsystem<UMapSubSystem>())
			{
				MapSub->LevelClear();
			}
		}
	}
}

