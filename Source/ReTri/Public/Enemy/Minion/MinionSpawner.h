// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "MinionSpawner.generated.h"

UCLASS()
class RETRI_API AMinionSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMinionSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 스포너에 가까이 가면 미니언들을 스폰위치에 생성
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CreateMinion();
	UFUNCTION()
	void AddMinionDeathCount();
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Collision)
	class UBoxComponent* BoxComp;
	
	UPROPERTY(EditAnywhere, Category = Spawn)
	TSubclassOf<class AMinion> DawnFactory;
	UPROPERTY(EditAnywhere, Category = Spawn)
	TSubclassOf<class AMinion> DuskFactory;
	
	UPROPERTY(EditAnywhere, Category = Spawn)
	TArray<AActor*> SpawnPoints;
	
	UPROPERTY(EditAnywhere, Category = Spawn)
	int32 MaxlMinionCount = 7;
	UPROPERTY(VisibleAnywhere, Category = Spawn)
	int32 CurrentDeathCount = 0;
	
	bool bHasSpawned = false;
	
	UPROPERTY(EditAnywhere, Category = Spawn)
	FName SpawnPointTag;
};
