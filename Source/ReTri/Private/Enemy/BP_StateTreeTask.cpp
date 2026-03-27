// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BP_StateTreeTask.h"

#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyData.h"
#include "AIController.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "ReTri/ReTri.h"

UBP_StateTreeTask::UBP_StateTreeTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldCallTick = true;
}

EStateTreeRunStatus UBP_StateTreeTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);
	//UE_LOG(LogTemp, Warning, TEXT("==== Boss Attack Task 시작! ===="));
	
	
	SkillWaitTime = 0.0f;	// 다음 스킬 대기 시간
	
	AActor* OwnerActor = Cast<AActor>(Context.GetOwner());
	if (!OwnerActor) return EStateTreeRunStatus::Failed;
	
	AEnemyBase* Boss = Cast<AEnemyBase>(OwnerActor);
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (!Boss)
	{
		//UE_LOG(LogTemp, Warning, TEXT("==== 실패 ===="));
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
	
	// ==================================================
	
	// 거리 계산
	float Distance = FVector::Dist(Boss->GetActorLocation(), Player->GetActorLocation());
	
	// 현재 체력 비율
	float HPRatio = Boss->CurrentHP / Boss->MaxHP;
	int32 RealPhase = Boss->CurrentPhase;
	int32 FinalSkillIndex = -1;		// 최종 스킬 인덱스
	TArray<int32> SkillPool;		// 이번에 쓸 스킬 후보지
	
	
	// 체력에 따른 페이즈
	if (HPRatio <= 0.3f)			// 3페이즈
	{
		RealPhase = 3;
		SkillPool = { 5, 6, 7 };
	}
	else if (HPRatio <= 0.6f)		// 2페이즈
	{
		if (Boss && !Boss->bHasPlayedMirrorBlade)
		{
			FinalSkillIndex = 4;
			Boss->bHasPlayedMirrorBlade = true;
			RealPhase = 2;
		}
		else
		{
			RealPhase = 2;
			SkillPool = { 0, 2, 2, 3, 3, 5, 5 };
		}
	}
	else							// 1페이즈
	{
		RealPhase = 1;
		if (Distance >= 400.f && Distance <= 1000.f)
		{
			// 중거리면 돌진, 점프만
			SkillPool = { 1 }; // 1, 1, 3
		}
		else
		{
			// 가까우면 모든 스킬
			SkillPool = { 0, 1, 1};	//0, 1, 1, 2, 2, 3, 3
		}
	}
	
	if (SkillPool.Num() > 0 && FinalSkillIndex == -1)
	{
		int32 RandomIndex = FMath::RandRange(0, SkillPool.Num() - 1);
		FinalSkillIndex = SkillPool[RandomIndex];
	}
	
	// ==================================================
	
	// 결정된 인덱스로 스킬 실행
	if (Boss->BossSkills.IsValidIndex(FinalSkillIndex))
	{
		SkillDataHandle = Boss->BossSkills[FinalSkillIndex];
		Boss->CurrentPhase = RealPhase;
	
		if (!SkillDataHandle.IsNull())
		{
			FSkillDataTableRow* SkillInfo = SkillDataHandle.GetRow<FSkillDataTableRow>(TEXT(""));
			EDarkMoonSkillType SelectedSkill = static_cast<EDarkMoonSkillType>(FinalSkillIndex);
			
			if (SkillInfo) // && SkillInfo->MontageToPlay)
			{
				//UE_LOG(LogTemp, Warning, TEXT("==== [%d페이즈] [%s] 시전 중 (인덱스: %d) ===="), RealPhase, *SkillInfo->SkillName, FinalSkillIndex);
				
				SCREENLOG("==== [%d페이즈] ==== [%s] !!!! ====", RealPhase, *SkillInfo->SkillName);
				
				Boss->SetCurrentSkillDamage(SkillInfo->Damage);
				//Boss ->PlayAnimMontage(SkillInfo->MontageToPlay);
				
				if (SelectedSkill == EDarkMoonSkillType::Dash)
				{
					// 대쉬 스킬 차징
					Boss->StartCharging(Player);
				}
				
				switch (SelectedSkill)
				{
				case EDarkMoonSkillType::BasicAttack:
					ExecuteBasicAttack(Boss, Player, SkillInfo->MontageToPlay);
					break;
				
				case EDarkMoonSkillType::Dash:
					ExecuteDash(Boss, Player, SkillInfo->MontageToPlay);
					break;
				
				case EDarkMoonSkillType::Flash:
					ExecuteFlash(Boss, Player, SkillInfo->MontageToPlay);
					break;
				
				case EDarkMoonSkillType::JumpDown:
					ExecuteJumpDown(Boss, Player, SkillInfo->MontageToPlay);
					break;
				
				case EDarkMoonSkillType::MirrorBlade:
					ExecuteMirrorBlade(Boss, Player, SkillInfo->MontageToPlay);
					break;
				
				case EDarkMoonSkillType::PowerDashSword:
					ExecutePowerDashSword(Boss, Player, SkillInfo->MontageToPlay);
					break;
				
				case EDarkMoonSkillType::PowerDashShadow:
					ExecutePowerDashShadow(Boss, Player, SkillInfo->MontageToPlay);
					break;
				
				case EDarkMoonSkillType::PowerJumpDown:
					ExecutePowerJumpDown(Boss, Player, SkillInfo->MontageToPlay);
					break;
				}
				
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
			if (SkillWaitTime < 1.5f)
			{
				return  EStateTreeRunStatus::Running;
			}
			
			//UE_LOG(LogTemp, Warning, TEXT("공격 끗"))
			SkillWaitTime = 0.0f;
			return EStateTreeRunStatus::Succeeded;
		}
	}
	
	return EStateTreeRunStatus::Running;
}

void UBP_StateTreeTask::ExecuteBasicAttack(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
	Boss->PlayAnimMontage(Montage, 0.8);
}

void UBP_StateTreeTask::ExecuteDash(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
	if (!Boss || !Player || !Montage) return;
	
	// 보스가 플레이어를 보며 차징
	Boss->bIsCharging = true;
	Boss->TargetActor = Player;
	
	FTimerHandle DashTimer;
	Boss->GetWorldTimerManager().SetTimer(DashTimer, [Boss, Player, Montage]()
	{
		if (Boss && Player)
		{
			// 1.5초 후에 차징 풀리면
			Boss->bIsCharging = false;
			
			// 플레이어를 향하게, Z는 고정하고
			FVector Dir = (Player->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();
			Dir.Z = 0; 
			Boss->SetActorRotation(Dir.Rotation());
			
			// 발사!
			Boss->LaunchCharacter(Boss->GetActorForwardVector() * 10000.0f, true, false);
            
			Boss->PlayAnimMontage(Montage);
		}
	}, 1.5f, false);
}

void UBP_StateTreeTask::ExecuteFlash(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
}

void UBP_StateTreeTask::ExecuteJumpDown(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
}

void UBP_StateTreeTask::ExecuteMirrorBlade(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
}

void UBP_StateTreeTask::ExecutePowerDashSword(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
}

void UBP_StateTreeTask::ExecutePowerDashShadow(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
}

void UBP_StateTreeTask::ExecutePowerJumpDown(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
}

