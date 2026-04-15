// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "BP_StateTreeTask.generated.h"

class ADarkMoon;
class ACharacter;
class UAnimMontage;
class AenemyBase;

UENUM(BlueprintType)
enum class EDarkMoonSkillType : uint8
{
	BasicAttack,
	Dash,
	JumpDown,
	MirrorBlade,
	PowerDashSword,
	PowerJumpDown 
};

UCLASS(Blueprintable, meta=(DisplayName = "공격태스크"))
class RETRI_API UBP_StateTreeTask : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	UBP_StateTreeTask(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	
public:
	UPROPERTY(EditAnywhere, Category = SkillSettings)
	EDarkMoonSkillType CurrentSkill;

	
protected:
	// 공격 실행, 어떤 스킬 가져올 건지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle SkillDataHandle;
	
	
	UPROPERTY(EditAnywhere, Category = Settings)
	float SkillWaitTime = 0.0f;
	
public:
	// ----------- Dash -----------
	UPROPERTY(EditAnywhere, Category = Skill)
	float DashSpeed = 10000;

	// ----------- JumpDown -----------
	FVector LandingPosition;

	
	
private:
	int32 BladeRepeatCount = 0; // MirrorBlade 스킬 n회 반복 카운트 
	FTimerHandle CycleTimerHandle;
	FTimerHandle DashTimer;
	

private:
	void ExecuteBasicAttack(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage);
	void ExecuteDash(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage);
	void ExecuteJumpDown(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage);
	void ExecuteMirrorBlade(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage);
	void ExecutePowerDashSword(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage);
	void ExecutePowerJumpDown(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage);
	
	void ExecutePatternCycle(AEnemyBase* Boss);
};
