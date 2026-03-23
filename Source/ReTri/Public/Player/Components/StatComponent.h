// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Data/PlayerStatData.h"
#include "StatComponent.generated.h"

// 성소 시스템에서 어떤 스탯 올릴지 구분하는 enum
// 공격력, 쿨타임, 화염 피해
UENUM(BlueprintType)
enum class EStatTypes : uint8
{
	MoveSpeed,
	AttackDamage,
	AttackSpeed,
	DashCooldown,
	MaxHP,
	FireDamage,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, EStatTypes, StatType, float, NewValue);

UCLASS(ClassGroup=(Player), meta=(BlueprintSpawnableComponent))
class RETRI_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 성소 등 외부에서 스탯 강화 시
	UFUNCTION(BlueprintCallable, Category="Stats")
	void ApplyStatModifier(EStatTypes Type, float Delta);

	// 레벨에 맞는 기본 스탯을 DataTable에서 로드
	UFUNCTION(BlueprintCallable, Category="Stats")
	void LoadStatsForLevel(int32 Level);

	// 성소 등 외부 시스템에서 현재 스탯 전체를 읽을 때 사용
	UFUNCTION(BlueprintPure, Category="Stats")
	FPlayerStatInfo GetStatInfo() const;
	
	UFUNCTION(BlueprintPure, Category="Stats")
	float GetMoveSpeed() const { return MoveSpeed; }
	
	UFUNCTION(BlueprintPure, Category="Stats")
	float GetAttackDamage() const { return AttackDamage; }
	
	UFUNCTION(BlueprintPure, Category="Stats")
	float GetAttackSpeed() const { return AttackSpeed; }
	
	UFUNCTION(BlueprintPure, Category="Stats")
	float GetDashCooldown() const { return DashCooldown; }
	
	UFUNCTION(BlueprintPure, Category="Stats")
	float GetMaxHP() const { return MaxHP; }

	UFUNCTION(BlueprintPure, Category="Stats")
	float GetFireDamage() const { return FireDamage; }
	
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnStatChanged OnStatChanged;
	
private:
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float MoveSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float AttackDamage = 10.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float AttackSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float DashCooldown = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float MaxHP = 100.f;

	UPROPERTY(EditAnywhere, Category="Stats|Base", meta=(AllowPrivateAccess=true))
	float FireDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Stats|DataTable", meta=(AllowPrivateAccess=true))
	TObjectPtr<UDataTable> StatDataTable;

	UPROPERTY(VisibleAnywhere, Category="Stats|Level", meta=(AllowPrivateAccess=true))
	int32 CurrentLevel = 1;
};
