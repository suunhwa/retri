// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class RETRI_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 발사체
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* moveComp;
	
	// 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* collisionComp;
	
	// 외관
	UPROPERTY(VisibleAnywhere)  
	class UStaticMeshComponent* bodyMesh;
};
