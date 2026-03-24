// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BP_StateTreeTask.h"

#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyData.h"
#include "AIController.h"
#include "Enemy/EnemyBase.h"

UBP_StateTreeTask::UBP_StateTreeTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = true;
}

EStateTreeRunStatus UBP_StateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	// UE_LOG(LogTemp, Warning, TEXT("==== Boss Attack Task 시작! ===="));
	
	SkillWaitTime = 0.0f;
	
	AActor* OwnerActor = Cast<AActor>(Context.GetOwner());
	if (!OwnerActor) return EStateTreeRunStatus::Failed;
	
	AEnemyBase* Boss = Cast<AEnemyBase>(OwnerActor);
	
	if (!Boss)
	{
		if (AAIController* aic = Cast<AAIController>(OwnerActor))
		{
			Boss = Cast<AEnemyBase>(aic->GetPawn());
		}
	}
	if (!Boss)
	{
		UE_LOG(LogTemp, Error, TEXT("보스(Pawn)를 찾을 수 없습니다."));
		return EStateTreeRunStatus::Failed;
	}

	if (Boss->BossSkills.Num() < 8)
	{
		UE_LOG(LogTemp, Error, TEXT("보스 스킬 부족! (현재: %d개)"), Boss->BossSkills.Num());
		return EStateTreeRunStatus::Failed;
	}
	
	
	// 현재 체력 비율
	float HPRatio = Boss->CurrentHP / Boss->MaxHP;
	int32 FinalSkillIndex = -1;		// 최종 스킬 인덱스
	TArray<int32> SkillPool;	// 이번에 쓸 스킬 후보지
	
	// 체력에 따른 페이즈
	if (HPRatio <= 0.3f)				// 3페이즈
	{
		CurrentPhase = 3;
		SkillPool = { 5, 6, 7 };
	}
	else if (HPRatio <= 0.6f)		// 2페이즈
	{
		if (CurrentPhase < 2 && !bHasPlayedMirrorBlade)
		{
			// 2페이즈 진입 직후 MirrorBlade 스킬 확정 사용
			FinalSkillIndex = 4;
			bHasPlayedMirrorBlade = true;
			CurrentPhase = 2;
		}
		else
		{
			CurrentPhase = 2;
			SkillPool = { 0, 2, 2, 3, 3, 5, 5 };
		}
	}
	else							// 1페이즈
	{
		CurrentPhase = 1;
		SkillPool = { 0, 1, 1, 2, 2, 3, 3};
	}
	
	if (SkillPool.Num() > 0 && FinalSkillIndex == -1)
	{
		int32 RandomIndex = FMath::RandRange(0, SkillPool.Num() - 1);
		FinalSkillIndex = SkillPool[RandomIndex];
	}
	
	
	// 결정된 인덱스로 스킬 실행
	if (Boss->BossSkills.IsValidIndex(FinalSkillIndex))
	{
		SkillDataHandle = Boss->BossSkills[FinalSkillIndex];
	
		if (!SkillDataHandle.IsNull())
		{
			FSkillDataTableRow* SkillInfo = SkillDataHandle.GetRow<FSkillDataTableRow>(TEXT(""));
		
			if (SkillInfo && SkillInfo->MontageToPlay)
			{
				Boss->PlayAnimMontage(SkillInfo->MontageToPlay);
				UE_LOG(LogTemp, Warning, TEXT("==== [%d페이즈] [%s] 시전 중 (인덱스: %d) ===="), CurrentPhase,
				*SkillDataHandle.RowName.ToString(), FinalSkillIndex);
			
				return EStateTreeRunStatus::Running;
			}
		}
	}
	
	return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus UBP_StateTreeTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	
	AActor* OwnerActor = Cast<AActor>(Context.GetOwner());
	AEnemyBase* Boss = Cast<AEnemyBase>(OwnerActor);
	
	if (!Boss)
	{
		if (AAIController* aic = Cast<AAIController>(OwnerActor))
		{
			Boss = Cast<AEnemyBase>(aic->GetPawn());
		}
	}
	if (Boss)
	{
		if (Boss->GetCurrentMontage() != nullptr) return EStateTreeRunStatus::Running;
		
		if (Boss->GetCurrentMontage() == nullptr)
		{
			SkillWaitTime += DeltaTime;
			if (SkillWaitTime < 1.0f)
			{
				return  EStateTreeRunStatus::Running;
			}
			
			UE_LOG(LogTemp, Warning, TEXT("공격 끗"))
			SkillWaitTime = 0.0f;
			return EStateTreeRunStatus::Succeeded;
		}
		
		// if (Boss->GetCurrentMontage() == nullptr)
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("공격 끗"))
		// 	return EStateTreeRunStatus::Succeeded;
		// }
	}
	
	return EStateTreeRunStatus::Running;
}


