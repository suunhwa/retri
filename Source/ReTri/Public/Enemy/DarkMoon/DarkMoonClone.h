// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"

#include "DarkMoonClone.generated.h"

UCLASS()
class RETRI_API ADarkMoonClone : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADarkMoonClone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Collision)
	class UBoxComponent* SwordCollision = nullptr;
	
	UFUNCTION()
	void OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, category=Collision)
	void OnSwordCollision();
	UFUNCTION(BlueprintCallable, category=Collision)
	void OffSwordCollision();
	
	
};
