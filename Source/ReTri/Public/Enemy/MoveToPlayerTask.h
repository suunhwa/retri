// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "MoveToPlayerTask.generated.h"


UCLASS(Blueprintable, meta=(DisplayName = "이동태스크"))
class RETRI_API UMoveToPlayerTask : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

	
public:
	UMoveToPlayerTask(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly, Category = Setteings)
	float Speed = 600.f;
	
	float Distance;
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Input", meta = (BindingKind = "Input"))
	float DistanceFromEvaluator = 0.0f;
	
	
};
