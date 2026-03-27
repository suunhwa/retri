// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "BP_StateTreeEvaluator.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, meta=(DisplayName = "거리 계산기"))
class RETRI_API UBP_StateTreeEvaluator : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()
	
	// 나와 플레이어의 거리를 계산해서 StateTree에 전달한다.
	// 거리 계산, 전달할 변수, 타겟, 거리값
	
public:
	UBP_StateTreeEvaluator(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	
public:
	UPROPERTY(EditAnywhere, Category = "Output", meta = (BindingKind = "Output"))
	float DistanceToTarget = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
	AActor* Target;
	
	
};
