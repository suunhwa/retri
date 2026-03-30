// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ReTri/ReTri.h"
#include "GameFramework/Actor.h"
#include "LootBase.generated.h"

class UCapsuleComponent;

UCLASS()
class RETRI_API ALootBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Break();
	
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY()
	bool bIsBroken = false;
	
	UPROPERTY()
	int32 HP = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AGoodsBase> GoodsClass;
};
