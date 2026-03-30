// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyData.generated.h"

// 스킬 데이터
USTRUCT(BlueprintType)
struct FSkillDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	FString SkillName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	float Damage = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	float Range = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	class UAnimMontage* MontageToPlay = nullptr;
	
};

// Enemy 데이터
USTRUCT(BlueprintType)
struct FEnemyDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic)
	FName MonsterName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic)
	float MaxHP = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic)
	float MoveSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic)
	float BasicAttackCoolDown = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	class UAnimMontage* MinionAttackMontage = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic)
	float BossPhase2 = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic)
	float BossPhase3 = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatSkills)
	TArray<FDataTableRowHandle> BossSkillsID;
	
};