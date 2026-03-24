// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TestEnemyPlayer.generated.h"

UCLASS()
class RETRI_API ATestEnemyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestEnemyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CamComp;
	
public:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Move;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* imc_Player;
	
	void OnMove(const struct FInputActionValue& inputValue);
	
	FVector direction;
	
	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float speed = 600;
};
