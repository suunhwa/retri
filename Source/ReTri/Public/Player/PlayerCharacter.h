// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Bullet.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class RETRI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

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
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Movement")
	class UInputAction* ia_Move;
	
	// 기본 공격 (mouse left btn)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Attack")
	class UInputAction* ia_Attack;
	
	// 패시브 공격 (mouse right btn)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Attack")
	class UInputAction* ia_Secondary;
	
	// 스킬 (Q)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	class UInputAction* ia_SkillQ;
	
	// 스킬 (E)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	class UInputAction* ia_SkillE;
	
	// 스킬 (R)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	class UInputAction* ia_SkillR;
	
	UPROPERTY(EditDefaultsOnly, Category="Inputs")
	class UInputMappingContext* imc_Player;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	void OnMove(const struct FInputActionValue& inputValue);
	void OnAttack(const struct FInputActionValue& inputValue);
	void OnSecondary(const struct FInputActionValue& inputValue);
	void OnSkillQ(const struct FInputActionValue& inputValue);
	void OnSkillE(const struct FInputActionValue& inputValue);
	void OnSkillR(const struct FInputActionValue& inputValue);
	
	FVector direction;
	
	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float speed = 600;
	
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<ABullet> BulletClass;
};
