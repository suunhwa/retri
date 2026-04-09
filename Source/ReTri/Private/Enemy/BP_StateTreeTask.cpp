// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BP_StateTreeTask.h"

#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyData.h"
#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	// UE_LOG(LogTemp, Warning, TEXT("==== Boss Attack Task 시작! ===="));
	
	
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
		// UE_LOG(LogTemp, Error, TEXT("보스(Pawn)를 찾을 수 없습니다."));
		return EStateTreeRunStatus::Failed;
	}

	if (Boss->BossSkills.Num() < 8)
	{
		// UE_LOG(LogTemp, Error, TEXT("보스 스킬 부족! (현재: %d개)"), Boss->BossSkills.Num());
		return EStateTreeRunStatus::Failed;
	}
	
	// ==================================================
	
	// 거리 계산
	float Distance = FVector::Dist(Boss->GetActorLocation(), Player->GetActorLocation());
	
	// 현재 체력 비율
	float HPRatio = Boss->CurrentHP / Boss->MaxHP;
	int32 RealPhase = Boss->CurrentPhase;
	int32 FinalSkillIndex = -1;		// 최종 스킬 인덱스
	TArray<int32> SkillPool;		// 이번에 쓸 스킬 후보지 / 2, 5 취소
	
	
	// 체력에 따른 페이즈
	if (HPRatio <= 0.3f)			// 3페이즈
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		Boss->Phase3VFX,
		FVector(Boss->GetActorLocation().X, Boss->GetActorLocation().Y, Boss->GetActorLocation().Z - 700.f),
		Boss->GetActorRotation(),
		FVector(4.0f),
		true,
		true,
		ENCPoolMethod::None,
		true
		);
		
		
		RealPhase = 3;
		SkillPool = { 6, 7 };
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
			SkillPool = { 0, 1, 1, 3, 3, 6, 6 };
		}
	}
	else							// 1페이즈
	{
		RealPhase = 1;
		if (Distance >= 400.f && Distance <= 1000.f)
		{
			// 중거리
			SkillPool = { 1, 1, 3 }; // 1, 1, 3
		}
		else if (Distance < 400.f)
		{
			// 근거리
			SkillPool = { 0, 1, 1 };
		}
		else
		{
			SkillPool = { 3 };
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
				
				// SCREENLOG("==== [%d페이즈] ==== [%s] !!!! ====", RealPhase, *SkillInfo->SkillName);
				
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
		
		if (CurrentSkill == EDarkMoonSkillType::MirrorBlade && BladeRepeatCount < 4) 
		{
			return EStateTreeRunStatus::Running;
		}
		
		SkillWaitTime += DeltaTime;
		if (SkillWaitTime < 1.5f) return  EStateTreeRunStatus::Running;
		
		if (Boss->GetCurrentMontage() == nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("공격 끗"))
			SkillWaitTime = 0.0f;
			Boss->RotateToTarget(DeltaTime, 3.0f);
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
			
			
			// 이펙트
			Boss->DashTrailComp->Deactivate();
			Boss->DashTrailComp->SetAbsolute(false, true, false);
			
			FRotator DashRot = Boss->GetActorForwardVector().Rotation();
			DashRot.Pitch += 90.f;
			Boss->DashTrailComp->SetWorldRotation(DashRot);
			
			Boss->DashTrailComp->ResetSystem();
			Boss->DashTrailComp->Activate(true);
			
			// 카메라
			if (Boss->DashCameraShake)
			{
				APlayerController* pc = Boss->GetWorld()->GetFirstPlayerController();
				if (pc)
				{
					pc->ClientStartCameraShake(Boss->DashCameraShake);
				}
			}

			Boss->PlayAnimMontage(Montage);
		}
	}, 1.5f, false);
	
}

void UBP_StateTreeTask::ExecuteFlash(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
}

void UBP_StateTreeTask::ExecuteJumpDown(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
	if (!Boss || !Player || !Montage) return;
	
	Boss->bIsJumpDownAttacking = true;
	Boss->bIsEnhancedJump = false;
	
	// 플레이어 위치
	LandingPosition = Player->GetActorLocation();
	
	Boss->PlayAnimMontage(Montage);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		Boss->JumpVFX,
		FVector(Boss->GetActorLocation().X, Boss->GetActorLocation().Y, Boss->GetActorLocation().Z - 700.f),
		Boss->GetActorRotation(),
		FVector(4.0f),
		true,
		true,
		ENCPoolMethod::None,
		true
		);
	
	Boss->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Boss->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
	FVector SkyLoc = Boss->GetActorLocation() + FVector(0, 0, 3000.f); 
	Boss->SetActorLocation(SkyLoc);
	Boss->SetActorHiddenInGame(true); // 보스 숨기기
	
	
	// 보스야 장판 생성해
	Boss->SpawnJumpDecal(LandingPosition, Boss->JumpCircleDecal);
}

void UBP_StateTreeTask::ExecuteMirrorBlade(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
	if (!Boss || !Player || !Montage) return;
	
	Boss->SpawnClones();
	
	BladeRepeatCount = 0;	// 카운트 초기화
	
	Boss->PlayAnimMontage(Montage, 0.2);

	// 패턴 시작
	ExecutePatternCycle(Boss);	
}

void UBP_StateTreeTask::ExecutePatternCycle(AEnemyBase* Boss)
{
	if (!IsValid(Boss)) return;

	if (BladeRepeatCount == 0)
	{
		Boss->PlayAnimMontage(Boss->MirrorBladeMontage, 0.5);
	}
	
	if (BladeRepeatCount >= 4)
	{
		Boss->SetActorHiddenInGame(false);
		return;
	}

	const int32 CloneNum = Boss->ActiveClones.Num();
	if (CloneNum <= 0) return;

	const float AppearanceInterval = 0.1f;   // 장판 순차 등장 간격
	const float WaitBeforeProgress = 0.3f;   // 다 등장한 뒤 잠깐 대기
	const float ProgressDuration = 1.0f;     // 차오르는 시간
	const float NextCycleDelay = 0.8f;       // 다음 반복까지 텀

	// =========================
	// 1) 장판 순차 등장
	for (int32 i = 0; i < CloneNum; ++i)
	{
		const float Delay = i * AppearanceInterval;
		AEnemyBase* Clone = Boss->ActiveClones[i];

		FTimerHandle Handle;
		Boss->GetWorldTimerManager().SetTimer(
			Handle,
			[Clone]()
			{
				if (IsValid(Clone))
				{
					Clone->InitCloneDecal(110.f, 2400.f);
				}
			},
			Delay,
			false
		);
	}

	// 마지막 장판이 등장하고, 잠깐 쉰 뒤 차오름 시작
	const float StartProgressTime = (CloneNum - 1) * AppearanceInterval + WaitBeforeProgress;

	// =========================
	// 2) 동시에 차오름 시작
	FTimerHandle StartProgressHandle;
	Boss->GetWorldTimerManager().SetTimer(
		StartProgressHandle,
		[this, Boss, ProgressDuration]()
		{
			if (!IsValid(Boss)) return;

			TSharedPtr<FTimerHandle> ProgressHandlePtr = MakeShared<FTimerHandle>();
			TSharedPtr<float> Elapsed = MakeShared<float>(0.0f);

			Boss->GetWorldTimerManager().SetTimer(
				*ProgressHandlePtr,
				[this, Boss, ProgressDuration, ProgressHandlePtr, Elapsed]() mutable
				{
					if (!IsValid(Boss)) return;

					*Elapsed += 0.02f;
					const float Progress = FMath::Clamp(*Elapsed / ProgressDuration, 0.0f, 1.0f);

					for (AEnemyBase* Clone : Boss->ActiveClones)
					{
						if (IsValid(Clone) && IsValid(Clone->DynamicBoxDecal))
						{
							Clone->DynamicBoxDecal->SetScalarParameterValue(TEXT("BoxProgress"), Progress);
						}
					}

					if (Progress >= 1.0f)
					{
						Boss->GetWorldTimerManager().ClearTimer(*ProgressHandlePtr);
					}
				},
				0.02f,
				true
			);
		},
		StartProgressTime,
		false
	);

	// =========================
	// 3) 다 차면 동시에 폭발
	const float ExplosionTime = StartProgressTime + ProgressDuration;

	FTimerHandle ExplosionHandle;
	Boss->GetWorldTimerManager().SetTimer(
		ExplosionHandle,
		[this, Boss, NextCycleDelay]()
		{
			if (!IsValid(Boss)) return;

			for (AEnemyBase* Clone : Boss->ActiveClones)
			{
				if (IsValid(Clone))
				{
					Boss->ExecuteMirrorBladeDamage(Clone);

					if (IsValid(Clone->ActiveDecal))
					{
						Clone->ActiveDecal->DestroyComponent();
						Clone->ActiveDecal = nullptr;
					}

					Clone->Destroy();
				}
			}
			
			// 카메라
			if (Boss->DashCameraShake)
			{
				APlayerController* pc = Boss->GetWorld()->GetFirstPlayerController();
				if (pc)
				{
					pc->ClientStartCameraShake(Boss->DashCameraShake);
				}
			}
			
			// 사운드
			UGameplayStatics::PlaySoundAtLocation(Boss, Boss->MirrorBladeSFX, Boss->GetActorLocation());
			

			Boss->ActiveClones.Empty();
			BladeRepeatCount++;

			FTimerHandle NextHandle;
			Boss->GetWorldTimerManager().SetTimer(
				NextHandle,
				[this, Boss]()
				{
					if (!IsValid(Boss)) return;

					if (BladeRepeatCount < 4)
					{
						Boss->SpawnClones();
						
						ExecutePatternCycle(Boss);
					}
					else
					{
						Boss->SetActorHiddenInGame(false);
					}
				},
				NextCycleDelay,
				false
			);
		},
		ExplosionTime,
		false
	);
}

void UBP_StateTreeTask::ExecutePowerDashSword(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
	if (!Boss || !Player || !Montage) return;
    
	Boss->bIsCharging = true;
	Boss->TargetActor = Player;
	
	Boss->GetWorldTimerManager().SetTimer(DashTimer, [Boss, Player, Montage]()
	{
	   if (IsValid(Boss) && IsValid(Player))
	   {
		  Boss->bIsCharging = false;
          
		  FVector Dir = (Player->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();
		  Dir.Z = 0; 
		  Boss->SetActorRotation(Dir.Rotation());
	   	
		  Boss->ExecuteReinforcedDash(Boss->GetActorLocation(), Boss->GetActorRotation());

		  Boss->LaunchCharacter(Boss->GetActorForwardVector() * 10000.0f, true, false);
		  Boss->PlayAnimMontage(Montage);
	   	
	   		// 이펙트
			Boss->DashTrailComp->Deactivate();
			Boss->DashTrailComp->SetAbsolute(false, true, false);
			
			FRotator DashRot = Boss->GetActorForwardVector().Rotation();
			DashRot.Pitch += 90.f;
			Boss->DashTrailComp->SetWorldRotation(DashRot);
			
			Boss->DashTrailComp->ResetSystem();
			Boss->DashTrailComp->Activate(true);
	   }
	}, 1.5f, false);
}

void UBP_StateTreeTask::ExecutePowerDashShadow(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
	
}

void UBP_StateTreeTask::ExecutePowerJumpDown(AEnemyBase* Boss, ACharacter* Player, UAnimMontage* Montage)
{
	if (!Boss || !Player || !Montage) return;
	
	Boss->bIsJumpDownAttacking = true;
	Boss->bIsEnhancedJump = true;

	LandingPosition = Player->GetActorLocation();

	Boss->PlayAnimMontage(Montage);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		Boss->EnhancedJumpVFX,
		FVector(Boss->GetActorLocation().X, Boss->GetActorLocation().Y, Boss->GetActorLocation().Z - 700.f),
		Boss->GetActorRotation(),
		FVector(4.0f),
		true,
		true,
		ENCPoolMethod::None,
		true
		);

	Boss->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	Boss->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FVector SkyLoc = Boss->GetActorLocation() + FVector(0, 0, 3000.f);
	Boss->SetActorLocation(SkyLoc);
	Boss->SetActorHiddenInGame(true);

	Boss->SpawnEnhancedJumpDecal(LandingPosition, Boss->JumpCircleDecal, Boss->JumpCrossDecal);
}


