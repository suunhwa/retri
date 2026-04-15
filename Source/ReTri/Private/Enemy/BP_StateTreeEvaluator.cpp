// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BP_StateTreeEvaluator.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Actor.h"


UBP_StateTreeEvaluator::UBP_StateTreeEvaluator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Target = nullptr;
	DistanceToTarget = 0.0f;
}

void UBP_StateTreeEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	
	// 주인 알아오기
	AActor* OwnerActor = Cast<AActor>(Context.GetOwner());
	if (OwnerActor != nullptr && Target != nullptr)
	{
		FVector A = Target->GetActorLocation();
		FVector B = OwnerActor->GetActorLocation();
		
		// 거리
		DistanceToTarget = (A - B).Size();
	}
		
}

