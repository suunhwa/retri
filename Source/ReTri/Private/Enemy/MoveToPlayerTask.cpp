// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/MoveToPlayerTask.h"
#include "GameFramework/CharacterMovementComponent.h"

EStateTreeRunStatus UMoveToPlayerTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{ 
	return Super::EnterState(Context, Transition);
}

EStateTreeRunStatus UMoveToPlayerTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	return Super::Tick(Context, DeltaTime);
}
