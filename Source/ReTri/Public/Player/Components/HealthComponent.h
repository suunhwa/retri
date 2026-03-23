// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, CurrentHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AController*, Killer);

UCLASS(ClassGroup=(Player), meta=(BlueprintSpawnableComponent))
class RETRI_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHPChanged OnHPChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnDeath OnDeath;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	float HandleDamage(float DamageAmount, AController* Instigator);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float Amount);
	
	UFUNCTION(BlueprintPure, Category="Health")
	float GetCurrentHP() const { return CurrentHP; }
	
	UFUNCTION(BlueprintPure, Category="Health")
	float GetMaxHP() const { return MaxHP; }
	
	UFUNCTION(BlueprintPure, Category="Health")
	bool IsDead() const { return bIsDead; }
	
	// 성소/강화에서 MaxHP 올릴 때 사용
	void SetMaxHP(float NewMaxHP, bool bHealToFull = false);
	
private:
	UPROPERTY(EditAnywhere, Category="Health", meta=(AllowPrivateAccess="true"))
	float MaxHP = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Health", meta=(AllowPrivateAccess="true"))
	float CurrentHP = 100.f;
	
	bool bIsDead = false;
};
