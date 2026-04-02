// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReTri/ReTri.h"
#include "ReTriGameData.generated.h"

//! PlayerStats 
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurseMoves = 1;		// 저주 
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<스킬> 인벤토리 스킬;
};

//! Loot Info 
USTRUCT(BlueprintType)
struct FLootsInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinGold = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxGold = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinDreamPowder = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDreamPowder = 100;
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

	//! Stat 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerStats PlayerStats;
	UFUNCTION(BlueprintCallable)
	FPlayerStats GetPlayerStats() { return PlayerStats; }
	UFUNCTION(BlueprintCallable)
	void SetPlayerStats(FPlayerStats CurStats) { PlayerStats = CurStats; }
	
	//! Loot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLootsInfo LootsInfo;
	
public:
	// === GET ===
	// 기본 재화 및 경험치
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	int32 GetCurGold() const { return PlayerStats.Gold; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	int32 GetCurDreamPowder() const { return PlayerStats.DreamPowder; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	float GetCurExp() const { return PlayerStats.Exp; }

	// 체력 관련
	UFUNCTION(BlueprintCallable, Category = "Stats|Health")
	float GetMaxHP() const { return PlayerStats.MaxHP; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Health")
	float GetCurHP() const { return PlayerStats.CurrentHP; }

	// 전투 스탯
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	float GetCurAttackDamage() const { return PlayerStats.AttackPower; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	float GetCurAttackSpeed() const { return PlayerStats.AttackSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	float GetCurAbilityPower() const { return PlayerStats.AbilityPower; }

	// 보조 및 유틸리티
	UFUNCTION(BlueprintCallable, Category = "Stats|Utility")
	float GetCurMoveSpeed() const { return PlayerStats.MoveSpeed; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Utility")
	float GetCurCoolTime() const { return PlayerStats.CoolTime; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Elemental")
	float GetCurFireDamage() const { return PlayerStats.FireDamage; }
	
	// Loot 관련 함수
	UFUNCTION(BlueprintCallable, Category="LootInfo")
	int32 GetRandomGold() const { return FMath::RandRange(LootsInfo.MinGold, LootsInfo.MaxGold); }
	UFUNCTION(BlueprintCallable, Category="LootInfo")
	int32 GetRandomDreamPowder() const { return FMath::RandRange(LootsInfo.MinDreamPowder, LootsInfo.MaxDreamPowder); }
	UFUNCTION(BlueprintCallable, Category="LootInfo")
	int32 GetMaxGold() const { return LootsInfo.MaxGold; }
	UFUNCTION(BlueprintCallable, Category="LootInfo")
	int32 GetMaxDreamPowder() const { return LootsInfo.MaxDreamPowder; }
	
	// === SET ===
	// 기본 재화 및 경험치, 체력 
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	void UpdateGold(int32 AmountGold) { PlayerStats.Gold += AmountGold; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	void UpdateDreamPowder(int32 AmountDreamPowder) { PlayerStats.DreamPowder += AmountDreamPowder; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Currency")
	void UpdateExp(float AmountExp) { PlayerStats.Exp += AmountExp; }
	
	UFUNCTION(BlueprintCallable, Category = "Stats|Health")
	void UpdateHP(float AmountHP) { PlayerStats.CurrentHP += AmountHP; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Health")
	void UpdateMaxHP(float AmountHP) { PlayerStats.MaxHP += AmountHP; }
	
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	void UpdateAttackDamage(float Amount) { PlayerStats.AttackPower += Amount; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	void UpdateAttackSpeed(float Amount) { PlayerStats.AttackSpeed += Amount; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Combat")
	void UpdateAbilityPower(float Amount) { PlayerStats.AbilityPower += Amount; }
	
	UFUNCTION(BlueprintCallable, Category = "Stats|Utility")
	void UpdateMoveSpeed(float Amount) { PlayerStats.MoveSpeed += Amount; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Utility")
	void UpdateCoolTime(float Amount) { PlayerStats.CoolTime += Amount; }
	UFUNCTION(BlueprintCallable, Category = "Stats|Elemental")
	void UpdateFireDamage(float Amount) { PlayerStats.FireDamage += Amount; }
	
	/*void DebugStat() const
	{
		UE_LOG(jiwon, Log, TEXT("==================== Player Stats ===================="));
		// 기본 스탯
		UE_LOG(jiwon, Log, TEXT("[Base Stats] HP: %.2f / AD: %.2f / AP: %.2f"), PlayerStats.CurrentHP, PlayerStats.AttackPower, PlayerStats.AbilityPower);
		UE_LOG(jiwon, Log, TEXT("[Base Stats] Gold: %d / DreamPowder: %d / Exp: %.2f"), PlayerStats.Gold, PlayerStats.DreamPowder, PlayerStats.Exp);
		// 보조 스탯
		UE_LOG(jiwon, Log, TEXT("[Secondary Stats] AS: %.2f / CoolTime: %.2f"), PlayerStats.AttackSpeed, PlayerStats.CoolTime);
		// 유틸리티 및 원소 스탯
		UE_LOG(jiwon, Log, TEXT("[Utility] MoveSpeed: %.2f"), PlayerStats.MoveSpeed);
		UE_LOG(jiwon, Log, TEXT("[Elemental] FireDamage: %.2f"), PlayerStats.FireDamage);
		UE_LOG(jiwon, Log, TEXT("======================================================"));
	}*/
};
