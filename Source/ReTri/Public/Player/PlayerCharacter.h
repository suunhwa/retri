// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UStatComponent;
class UAbilityComponent;
class ABullet;
class UInputAction;
class UInputMappingContext;

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
	
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	
private:
	// Components
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CamComp;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UHealthComponent> HealthComp;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UAbilityComponent> AbilityComp;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStatComponent> StatComp;
	

private:
	// Input Bindings
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Movement")
	TObjectPtr<UInputAction> ia_Move;
	
	// 기본 공격 (mouse left btn)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Attack")
	TObjectPtr<UInputAction> ia_Attack;
	
	// 패시브 공격 (mouse right btn)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Attack")
	TObjectPtr<UInputAction> ia_Secondary;
	
	// 스킬 (Q)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	TObjectPtr<UInputAction> ia_SkillQ;
	
	// 스킬 (E)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	TObjectPtr<UInputAction> ia_SkillE;
	
	// 스킬 (R)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	TObjectPtr<UInputAction> ia_SkillR;
	
	UPROPERTY(EditDefaultsOnly, Category="Inputs")
	TObjectPtr<UInputMappingContext> imc_Player;

private:
	// Attack
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<ABullet> BulletClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

private:
	// Input Handlers
	void OnMove(const struct FInputActionValue& inputValue);
	void OnAttack(const struct FInputActionValue& inputValue);
	void OnSecondary(const struct FInputActionValue& inputValue);
	void OnSkillQ(const struct FInputActionValue& inputValue);
	void OnSkillE(const struct FInputActionValue& inputValue);
	void OnSkillR(const struct FInputActionValue& inputValue);
	void OnDash(const struct FInputActionValue& inputValue);
	
private:
	UFUNCTION()
	void HandleDash(AController* Killer);
	
	/*FVector direction;
	
	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float speed = 600;*/
};
