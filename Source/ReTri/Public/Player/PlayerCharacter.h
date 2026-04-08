// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "UI/HPBar.h"
#include "UI/PlayerHUD.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UStatComponent;
class UAbilityComponent;
class ABullet;
class UInputAction;
class UInputMappingContext;
class UNiagaraSystem;
class UParticleSystem;
class ABossDropItem;
class UCameraShakeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackCountChanged, int32, Count);

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
	
public:
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

// getter (성소 상호작용)
public:
	UFUNCTION(BlueprintPure, Category="Components")
	UHealthComponent* GetHealthComponent() const { return HealthComp; }
	
	UAbilityComponent* GetAbilityComponent() const { return AbilityComp; }
	UStatComponent* GetStatComponent() const { return StatComp; }

private:
	// Input Bindings
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Movement")
	TObjectPtr<UInputAction> ia_Move;
	
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Movement")
	TObjectPtr<UInputAction> ia_Dash;
	
	// 기본 공격 (mouse left btn)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Attack")
	TObjectPtr<UInputAction> ia_Attack;
	
	// 여행자 기억 1 - 핸드 캐논 (mouse right btn)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	TObjectPtr<UInputAction> ia_TravelerMemory1;

	// 스킬 (Q)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	TObjectPtr<UInputAction> ia_SkillQ;

	// 스킬 (E)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	TObjectPtr<UInputAction> ia_SkillE;

	// 여행자 기억 2 - 빠른 손 (R)
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Skills")
	TObjectPtr<UInputAction> ia_TravelerMemory2;
	
	// Interaction
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Interaction")
	TObjectPtr<UInputAction> ia_Interaction;
	
	// item pick up
	UPROPERTY(EditDefaultsOnly, Category="Inputs|Interaction")
	TObjectPtr<UInputAction> ia_Salvage;
	
	UPROPERTY(EditDefaultsOnly, Category="Inputs")
	TObjectPtr<UInputMappingContext> imc_Player;

private:
	// Attack
	UPROPERTY(EditAnywhere, Category=Attack)
	TSubclassOf<ABullet> BulletClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	// 레벨업 이펙트 (1~4레벨)
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	TObjectPtr<UNiagaraSystem> LevelUpEffect;

	// 중간 레벨업 이펙트 (5~9레벨)
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	TObjectPtr<UNiagaraSystem> MidLevelEffect;

	// 최대 레벨(10) 달성 이펙트
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	TObjectPtr<UNiagaraSystem> MaxLevelEffect;

	// 머리 위 이펙트 소켓 이름
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	FName LevelUpSocketName = TEXT("head");

	// 레벨업 이펙트 재생 시간 (초)
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	float LevelUpEffectDuration = 2.f;

	// 레벨업 이펙트 스케일 (X=가로, Y=가로, Z=세로)
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	FVector LevelUpEffectScale = FVector(1.f, 1.f, 1.f);

	// 레벨업 사운드 (1~4)
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	TObjectPtr<USoundBase> LevelUpSound;

	// 중간 레벨업 사운드 (5~9)
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	TObjectPtr<USoundBase> MidLevelSound;

	// 최대 레벨 사운드 (10)
	UPROPERTY(EditDefaultsOnly, Category="Effects|LevelUp")
	TObjectPtr<USoundBase> MaxLevelSound;

	// 4타 강화탄 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> EnhancedShotEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> EnhancedShotParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> EnhancedMuzzleParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	float EnhancedShotEffectScale = 1.f;

	// 4타 강화탄 카메라 셰이크
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UCameraShakeBase> EnhancedShotCS;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	// 사망 애니메이션 길이 (초) - 이 시간 후 게임 일시정지
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float DeathAnimDuration = 2.3f;
	
public:
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnAttackCountChanged OnAttackCountChanged;
	
	UPROPERTY(EditAnywhere, Category="Combat|Left")
	int32 AttackCount = 0;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bIsCombat = false;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float CombatExitDelay = 2.0f;

	float LastCombatTime = -100.f;

	UPROPERTY(EditAnywhere, Category="Combat|Left")
	float EnhancedShotMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, Category="Combat|Left")
	float AttackInterval = 0.2f;

	bool bCanAttack = true;

	FTimerHandle AttackTimerHandle;

private:
	// Input Handlers
	void OnMove(const struct FInputActionValue& inputValue);
	void OnDash(const struct FInputActionValue& inputValue);
	void EnterCombat();
	void OnAttack(const struct FInputActionValue& inputValue);
	void ResetAttack();
	void OnTravelerMemory1(const struct FInputActionValue& inputValue);
	void OnSkillQ(const struct FInputActionValue& inputValue);
	void OnSkillE(const struct FInputActionValue& inputValue);
	void OnTravelerMemory2(const struct FInputActionValue& inputValue);
	void OnInteraction(const struct FInputActionValue& inputValue);
	void OnPickUp(const struct FInputActionValue& inputValue);
	void OnSalvage(const struct FInputActionValue& inputValue);
	
private:
	UFUNCTION()
	void HandleDeath(AController* Killer);

	UFUNCTION()
	void HandleLevelUp(int32 NewLevel);
	
	/*FVector direction;
	
	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float speed = 600;*/
	
public:
	// 마우스로 Interaction Object Hover했을 때 띄울 UI
	UFUNCTION(BlueprintCallable)
	void HoverInteractable();
	
	// F를 눌렀을 때 실행 할 함수
	UFUNCTION(BlueprintCallable)
	void Interaction();
	
	// hp bar
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UWidgetComponent> HPBarComp;
	
	// 화면 좌하단 HUD
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	
	UFUNCTION(Exec)
	void DebugAddExp(int32 Amount);

private:
	UPROPERTY()
	TObjectPtr<UPlayerHUD> PlayerHUDWidget;
};
