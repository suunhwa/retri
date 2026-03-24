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
	
	}

EStateTreeRunStatus UBP_StateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	// UE_LOG(LogTemp, Warning, TEXT("==== Boss Attack Task 시작! ===="));
	
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
	
	// // 주인 알아오기
	// UObject* RawOwner = Context.GetOwner();
	// if (!RawOwner) return EStateTreeRunStatus::Failed;
	//
	// AActor* OwnerActor = Cast<AActor>(RawOwner);
	// ACharacter* OwnerChar = nullptr;
	// if (AAIController* aic = Cast<AAIController>(OwnerActor))
	// {
	// 	OwnerChar = Cast<ACharacter>(aic->GetPawn());
	// }
	//
	// if (OwnerChar == nullptr)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("OwnerChar is nullptr (보스 못 찾음)"));
	// 	return EStateTreeRunStatus::Failed;
	// }
	//
	//
	// AActor* OwnerEnemy = Cast<AActor>(Context.GetOwner());
	// AEnemyBase* Boss = nullptr;
	// if (AController* aic = Cast<AController>(OwnerActor))
	// {
	// 	Boss = Cast<AEnemyBase>(aic->GetPawn());
	// }
	// if (!Boss || Boss->BossSkills.Num() < 8)
	// {
	// 	return EStateTreeRunStatus::Failed;
	// }
	
	// 현재 체력 비율
	float HPRatio = Boss->CurrentHP / Boss->MaxHP;
	int32 FinalSkillIndex = 0;		// 최종 스킬 인덱스
	TArray<int32> SkillPool;	// 이번에 쓸 스킬 후보지
	
	// 체력에 따른 페이즈
	if (HPRatio <= 0.3)				// 3페이즈
	{
		CurrentPhase = 3;
		SkillPool = { 5, 6, 7 };
	}
	else if (HPRatio <= 0.6)		// 2페이즈
	{
		if (CurrentPhase < 2 && !bHasPlayedMirrorBlade)
		{
			// 2페이즈 진입 직후 MirrorBlade 스킬 사용
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
	
	if (SkillPool.Num() > 0 && FinalSkillIndex !=3)
	{
		int32 RandomIndex = FMath::RandRange(0, SkillPool.Num() - 1);
		FinalSkillIndex = SkillPool[RandomIndex];
	}
	
	// 결정된 인덱스로 스킬 실행
	FDataTableRowHandle SelectedHandle = Boss->BossSkills[FinalSkillIndex];
	if (!SelectedHandle.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("현재 체력 비율: %.2f | 선택된 스킬 이름: %s"), 
		   HPRatio, *SelectedHandle.RowName.ToString());
		
		FSkillDataTableRow* SkillInfo = SkillDataHandle.GetRow<FSkillDataTableRow>(TEXT("SkillInfo"));
		
		if (SkillInfo && SkillInfo->MontageToPlay)
		{
			Boss->PlayAnimMontage(SkillInfo->MontageToPlay);
			UE_LOG(LogTemp, Warning, TEXT("보스 스킬 실행: 인덱스 %d"), FinalSkillIndex);
			
			return EStateTreeRunStatus::Running;
		}
	}
	
	return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus UBP_StateTreeTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	
	AActor* OwnerActor = Cast<AActor>(Context.GetOwner());
	ACharacter* OwnerChar = nullptr;
	
	if (AController* aic = Cast<AController>(OwnerActor))
	{
		OwnerChar = Cast<ACharacter>(aic->GetPawn());
	}
	if (OwnerChar)
	{
		if (OwnerChar->GetCurrentMontage() == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("공격 끗"))
			
			return EStateTreeRunStatus::Succeeded;
		}
	}
	
	return EStateTreeRunStatus::Running;
}


