// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "BP_StateTreeTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(DisplayName = "공격태스크"))
class RETRI_API UBP_StateTreeTask : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	UBP_StateTreeTask(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	
	
protected:
	// 공격 실행, 어떤 스킬 가져올 건지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle SkillDataHandle;
	
	// 페이즈
	int32 CurrentPhase = 1;
	bool bHasPlayedMirrorBlade = false;
	
	UPROPERTY(EditAnywhere, Category = Settings)
	float SkillWaitTime = 0.0f;
	
	
	
};
