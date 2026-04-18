// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Data/PlayerSkillData.h"
#include "ReTriGameData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerPlayData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerDead = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 VisitLevel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KillEnemy = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KillBoss = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GiveDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GetDamage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Heal = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Gold = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DreamDust = 0;
	
public:
	void Set(int32 InPlayerDead, int32 InVisitLevel, int32 InKillEnemy, int32 InKillBoss, int32 InGiveDamage, int32 InGetDamage, int32 InHeal, int32 InGold, int32 InDreamDust)
	{
		PlayerDead = InPlayerDead;
		VisitLevel = InVisitLevel;
		KillEnemy  = InKillEnemy;
		KillBoss   = InKillBoss;
		GiveDamage = InGiveDamage;
		GetDamage  = InGetDamage;
		Heal       = InHeal;
		Gold       = InGold;
		DreamDust  = InDreamDust;
	}
	void SetPlayerDead(int32 InPlayerDead) { PlayerDead += InPlayerDead; }
	void SetVisitLevel(int32 InVisitLevel) { VisitLevel += InVisitLevel; }
	void SetKillEnemy(int32 InKillEnemy)   { KillEnemy = InKillEnemy; }
	void SetKillBoss(int32 InKillBoss)     { KillBoss += InKillBoss; }
	void SetGiveDamage(int32 InGiveDamage) { GiveDamage += InGiveDamage; }
	void SetGetDamage(int32 InGetDamage)   { GetDamage += InGetDamage; }
	void SetHeal(int32 InHeal)             { Heal += InHeal; }
	void SetGold(int32 InGold)             { Gold += InGold; }
	void SetDreamDust(int32 InDreamDust)   { DreamDust += InDreamDust; }
};

// === Loot Info === 
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

// === Shop Item Info === 
USTRUCT(BlueprintType)
struct FShopItemSkillData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerSkillData> ItemSkillDatas;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RETRI_API UReTriGameData : public UActorComponent
{
	GENERATED_BODY()

public:	
	UReTriGameData();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLootsInfo LootsInfo;
	
public:
	UFUNCTION(BlueprintCallable, Category="LootInfo")
	int32 GetRandomGold() const { return FMath::RandRange(LootsInfo.MinGold, LootsInfo.MaxGold); }
	UFUNCTION(BlueprintCallable, Category="LootInfo")
	int32 GetRandomDreamPowder() const { return FMath::RandRange(LootsInfo.MinDreamPowder, LootsInfo.MaxDreamPowder); }
	UFUNCTION(BlueprintCallable, Category="LootInfo")
	int32 GetMaxGold() const { return LootsInfo.MaxGold; }
	UFUNCTION(BlueprintCallable, Category="LootInfo")
	int32 GetMaxDreamPowder() const { return LootsInfo.MaxDreamPowder; }
};
