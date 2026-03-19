// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BP_StateTreeTask.h"

#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyData.h"

UBP_StateTreeTask::UBP_StateTreeTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	{
	}

EStateTreeRunStatus UBP_StateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	
	// 주인 알아오기
	ACharacter* OwnerChar = Cast<ACharacter>(Context.GetOwner());
	if (OwnerChar == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	// 스킬 핸들이 유효한지
	if (SkillDataHandle.IsNull())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	FSkillDataTableRow* SkillInfo = SkillDataHandle.GetRow<FSkillDataTableRow>(TEXT("SkillInfo"));
	if (SkillInfo == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	if (SkillInfo->MontageToPlay != nullptr)
	{
		float Duration = OwnerChar->PlayAnimMontage(SkillInfo->MontageToPlay);
		
		if (Duration > 0.0f)
		{
			return EStateTreeRunStatus::Running;
		}
	}
	return EStateTreeRunStatus::Failed;
}


