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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill);
	FString SkillName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill);
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill);
	float Range;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill);
	class UAnimMontage* MontageToPlay;
	
};

// Enemy 데이터
USTRUCT(BlueprintType)
struct FEnemyDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic);
	float MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic);
	float MoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic);
	float BasicAttackCoolDown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic);
	float BossPhase2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatBasic);
	float BossPhase3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatSkills);
	FDataTableRowHandle BasicAttackID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatSkills)
	TArray<FDataTableRowHandle> BossSkillsID;
	
};