// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/MoveToPlayerTask.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


UMoveToPlayerTask::UMoveToPlayerTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = true;
}

EStateTreeRunStatus UMoveToPlayerTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{ 
	Super::EnterState(Context, Transition);
	
	APawn* BossPawn = Cast<APawn>(Context.GetOwner());
	if (!BossPawn)
	{
		if (AAIController* aic = Cast<AAIController>(Context.GetOwner()))
		{
			BossPawn = aic->GetPawn();
		}
	}
	if (BossPawn)
	{
		UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(BossPawn->GetMovementComponent());
		if (MoveComp)
		{
			MoveComp->MaxWalkSpeed = 600.0f;
			Speed = MoveComp->MaxWalkSpeed;
		}
		
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(Context.GetWorld(), 0);
		AAIController* aic = Cast<AAIController>(BossPawn->GetController());
		
		if (aic && Player)
		{
			aic->MoveToActor(Player, 900);
			return EStateTreeRunStatus::Running;
		}
	}
	
	return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus UMoveToPlayerTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	
	AActor* OwnerActor = Cast<AActor>(Context.GetOwner());
	APawn* MyPawn = Cast<APawn>(OwnerActor);

	if (!MyPawn)
	{
		if (AAIController* AIC = Cast<AAIController>(OwnerActor))
		{
			MyPawn = AIC->GetPawn();
		}
	}
	
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (MyPawn == Player)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (MyPawn && Player)
	{
		Distance = FVector::Dist(MyPawn->GetActorLocation(), Player->GetActorLocation());
		
		if (Distance <= 1000.0f) 
		{
			return EStateTreeRunStatus::Succeeded; 
		}
	}
	
	return EStateTreeRunStatus::Running;
}
