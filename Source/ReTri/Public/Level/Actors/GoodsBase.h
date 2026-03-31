// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReTri/ReTri.h"
#include "GameFramework/Actor.h"
#include "GoodsBase.generated.h"

class USphereComponent;
class UFloatingUI;

UCLASS()
class RETRI_API AGoodsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoodsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION(BlueprintCallable)
	void MoveAndRotate(float DeltaTime);
protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere)
	AActor* TargetPlayer;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YawRotSpeed = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollRotSpeed = 1500.f;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class AFloatingUIActor> FloatingUIActorClass;
};
