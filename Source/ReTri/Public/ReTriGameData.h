// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReTriGameData.generated.h"

// 다음 맵으로 넘길 Struct
USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Gold = 0;				// 골드
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DreamPowder = 0;		// 꿈가루
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Exp = 1;				// 경험치
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxHP = 100.0f;		// 최대 체력		
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentHP = 100.0f;	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AttackPower = 10.0f;	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AttackSpeed = 10.0f;	// 공격 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float AbilityPower = 10.0f;	// 주문력 (스킬의 피해량 및 효과 결정하는 핵심 계수)
	
	// 보조 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float MoveSpeed = 600.f;	// 이동속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CoolTime = 1;			// 기억 가속 (쿨타임) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float FireDamage = 1;		// 화염 데미지 계수
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<스킬> 인벤토리 스킬;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RETRI_API UReTriGameData : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReTriGameData();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerStats PlayerStats;
	
	UFUNCTION(BlueprintCallable)
	FPlayerStats GetPlayerStats() { return PlayerStats; }
	
public:
	/** --- 개별 통계 Get 함수들 --- */

	// 기본 재화 및 경험치
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	int32 GetGold() const { return PlayerStats.Gold; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	int32 GetDreamPowder() const { return PlayerStats.DreamPowder; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	float GetExp() const { return PlayerStats.Exp; }

	// 체력 관련
	UFUNCTION(BlueprintCallable, Category = "Stats|Health")
	float GetMaxHP() const { return PlayerStats.MaxHP; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Health")
	float GetCurrentHP() const { return PlayerStats.CurrentHP; }

	// 전투 스탯
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	float GetAttackPower() const { return PlayerStats.AttackPower; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	float GetAttackSpeed() const { return PlayerStats.AttackSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	float GetAbilityPower() const { return PlayerStats.AbilityPower; }

	// 보조 및 유틸리티
	UFUNCTION(BlueprintCallable, Category = "Stats|Utility")
	float GetMoveSpeed() const { return PlayerStats.MoveSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Utility")
	float GetCoolTime() const { return PlayerStats.CoolTime; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Elemental")
	float GetFireDamage() const { return PlayerStats.FireDamage; }
	
	// 스탯 변화 함수
	UFUNCTION(BlueprintCallable)
	void UpdateHP(float AmountHP) { PlayerStats.CurrentHP += AmountHP; }
	UFUNCTION(BlueprintCallable)
	void UpdateGold(int32 AmountGold) { PlayerStats.Gold += AmountGold; }
	UFUNCTION(BlueprintCallable)
	void UpdateDreamPowder(int32 AmountDreamPowder) { PlayerStats.DreamPowder += AmountDreamPowder; }
	UFUNCTION(BlueprintCallable)
	void UpdateExp(float AmountExp) { PlayerStats.Exp += AmountExp; }
	
	void DebugStat() const
	{
		UE_LOG(LogTemp, Log, TEXT("==================== Player Stats ===================="));
		// 기본 스탯
		UE_LOG(LogTemp, Log, TEXT("[Base Stats] HP: %.2f / AD: %.2f / AP: %.2f"), PlayerStats.CurrentHP, PlayerStats.AttackPower, PlayerStats.AbilityPower);
		UE_LOG(LogTemp, Log, TEXT("[Base Stats] Gold: %d / DreamPowder: %d / Exp: %.2f"), PlayerStats.Gold, PlayerStats.DreamPowder, PlayerStats.Exp);
		// 보조 스탯
		UE_LOG(LogTemp, Log, TEXT("[Secondary Stats] AS: %.2f / CoolTime: %.2f"), PlayerStats.AttackSpeed, PlayerStats.CoolTime);
		// 유틸리티 및 원소 스탯
		UE_LOG(LogTemp, Log, TEXT("[Utility] MoveSpeed: %.2f"), PlayerStats.MoveSpeed);
		UE_LOG(LogTemp, Log, TEXT("[Elemental] FireDamage: %.2f"), PlayerStats.FireDamage);
		UE_LOG(LogTemp, Log, TEXT("======================================================"));
	}
};
