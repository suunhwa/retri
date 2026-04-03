// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyBase.h"

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"

#include "Components/DecalComponent.h"
#include "Enemy/DarkMoon/DarkMoon.h"
#include "Enemy/BP_StateTreeTask.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{	
	// 데이터테이블 존재 && 이름 존재
	if (StatDataTable != nullptr && !EnemyRowName.IsNone())
	{
		// 엑셀에서 이름과 일치하는 줄 가져오기
		MyStatInfo = StatDataTable->FindRow<FEnemyDataTableRow>(EnemyRowName, TEXT("Enemy Row Name"));
		
		// 데이터를 찾았다면 내 몸에 적용
		if (MyStatInfo != nullptr)
		{
			MaxHP = MyStatInfo->MaxHP;
			CurrentHP = MaxHP;
			
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = MyStatInfo->MoveSpeed;
			}
			
			// BasicAttack = MyStatInfo->BasicAttackID;
			BossSkills = MyStatInfo->BossSkillsID;
			
			UE_LOG(LogTemp, Warning, TEXT("성공!! %s의 체력은 %f, 스킬은 %d개"), *EnemyRowName.ToString(), CurrentHP, BossSkills.Num());
		}
	}
	
	// Begin이 연쇄적으로 일어나서 FSM에 Null로 뜸
	Super::BeginPlay();
	
	FTimerHandle StartTimer;
	GetWorldTimerManager().SetTimer(StartTimer, [this]()
	{
		if (AAIController* aic = Cast<AAIController>(GetController()))
		{
			UStateTreeComponent* STComp = aic->FindComponentByClass<UStateTreeComponent>();
			if (STComp)
			{
				STComp->StartLogic();
			}
		}
	}, 0.2f, false);
}

void AEnemyBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	bIsJumpDownAttacking = false;
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// bIsCharging이 true일 때 플레이어 방향으로 회전
	if (bIsCharging && TargetActor)
	{
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(ChargeMontage))
		{
			PlayAnimMontage(ChargeMontage, 1.0f);
		}
	}
	
	if (bCanRotate)
	{
		RotateToTarget(DeltaTime, 10.0f);
	}

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AEnemyBase::OnAttackOverlap(AActor* OtherActor)
{
	if (bHasHitTarget || !OtherActor || OtherActor == this) return;
	
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player)
	{
		UGameplayStatics::ApplyDamage
		(
			OtherActor,           // 맞는 대상
			CurrentSkillDamage,   // Task에서 설정한 수치
			GetController(),      // 보스의 컨트롤러
			this,                 // 보스 자신
			nullptr               // 특별한 타입 없으면 null
		);
		
		bHasHitTarget = true;

		UE_LOG(LogTemp, Warning, TEXT("플레이어에게 %f 데미지를 입혔다!"), CurrentSkillDamage);
	}
}

float AEnemyBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.0f;
	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHP -= ActualDamage;
	UpdatePhase();
	
	UE_LOG(LogTemp, Warning, TEXT("%s 적중! 남은 체력: %f"), *EnemyRowName.ToString(), CurrentHP);
	

	return ActualDamage;
}

void AEnemyBase::PlayDeathEffect()
{
	if (DeathCameraShake)
	{
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		if (pc)
		{
			pc->ClientStartCameraShake(DeathCameraShake);
		}
	}
	
	if (DeathVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathVFX, GetActorLocation());
	}
}

void AEnemyBase::BroadcastDeath()
{
	OnMinionDieDelegate.Broadcast();
}


void AEnemyBase::StartCharging(AActor* NewTarget)
{
	if (NewTarget)
	{
		TargetActor = NewTarget;
		bIsCharging = true;
	}
}

void AEnemyBase::StopCharging()
{
	bIsCharging = false;
	TargetActor = nullptr;
}

void AEnemyBase::RotateToTarget(float DeltaTime, float InterpSpeed)
{
	if (!TargetActor) return; // 타겟이 없으면 중단

	// 보스 위치 -> 플레이어 위치로 향하는 방향 벡터를 구하고, Z는 0
	FVector LookDir = TargetActor->GetActorLocation() - GetActorLocation();
	LookDir.Z = 0.0f; 

	// 구한 방향을 회전값으로
	FRotator TargetRot = LookDir.Rotation();

	// 현재 회전에서 목표 회전까지 부드럽게 보간
	// IntterpSpeed = 회전속도
	FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, InterpSpeed);
        
	SetActorRotation(NewRot);
}

void AEnemyBase::SpawnJumpDecal(FVector Location, class UMaterialInterface* Decal)
{
	CircleDecal = UGameplayStatics::SpawnDecalAtLocation
		(
		GetWorld(),
		Decal,
		FVector(700.f,700.f,700.f),
		Location,
		FRotator(-90, 0, 0),
		2.0f
		);
	
	if (CircleDecal)
	{
		UMaterialInstanceDynamic* LocalDynamicDecal = CircleDecal->CreateDynamicMaterialInstance();
		
		float UpdateInterval = 0.02f;
		
		// FTimerHandle TimerHandle;
		
		// 1. 스마트 포인터를 사용하여 타이머 핸들을 생성 (람다 내부에서 자신의 타이머를 올바르게 Clear하기 위함)
		TSharedPtr<FTimerHandle> TimerHandlePtr = MakeShared<FTimerHandle>();
		// 2. 타이머 대기 도중 보스가 파괴(사망)될 때 생기는 크래시 방지용 약포인터
		TWeakObjectPtr<AEnemyBase> WeakThis = this;
		UWorld* WorldContext = GetWorld(); // 보스가 파괴된 후에도 타이머 관리자에 접근해 타이머를 끄기 위해 미리 캐싱
		
		
		// Progress 업데이트
		// 람다 캡쳐
		// GetWorldTimerManager().SetTimer(TimerHandle, [this, LocalDynamicDecal, Progress=0.0f, TimerHandle, Location]() mutable
		
		
		// Progress 업데이트 (TimerHandlePtr 캡처)
		WorldContext->GetTimerManager().SetTimer(*TimerHandlePtr, [WeakThis, WorldContext, LocalDynamicDecal, Progress=0.0f, TimerHandlePtr, Location]() mutable
		{
			
			// 보스가 이미 파괴되었거나 유효하지 않으면 타이머 즉시 종료
			if (!WeakThis.IsValid())
			{
				if (IsValid(WorldContext))
				{
					WorldContext->GetTimerManager().ClearTimer(*TimerHandlePtr);
				}
				return;
			}
			
			AEnemyBase* Boss = WeakThis.Get();
			
			
			
			if (!IsValid(LocalDynamicDecal))
			{
				Boss->GetWorldTimerManager().ClearTimer(*TimerHandlePtr);
				return;
			}
			
			Progress += (0.5f / (1.0f / 0.02f));
			float ClampedProgress = FMath::Min(Progress,0.5f);
			
			if (LocalDynamicDecal)
			{
				LocalDynamicDecal->SetScalarParameterValue("Progress Circle", ClampedProgress);
			}
			
			
			if (Progress >= 0.5f) // 0.5 크기까지 커지면 
			{
				
				// Progress = 0.0f; // 초기화
				Boss->GetWorldTimerManager().ClearTimer(*TimerHandlePtr);
				
				FVector DownStartLoc = Location + FVector(0.0f, 0.0f, 1500.0f);
				Boss->SetActorLocation(DownStartLoc, false);
				
				Boss->SetActorHiddenInGame(false);
				Boss->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				Boss->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
				
				Boss->LaunchCharacter(FVector(0, 0, -5000.f), true, true);
				
				Boss->PlayAnimMontage(Boss->DownMontage, 1.0f);
			}
		}, UpdateInterval, true);
	}
}

void AEnemyBase::ExecuteJumpDownDamage()
{
	FVector ImpactLocation = GetActorLocation();
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	bool bHit = UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		CurrentSkillDamage,
		ImpactLocation,
		JumpDownDamageRadius,
		nullptr,
		IgnoreActors,
		this,
		GetInstigatorController(),
		false
		);
	
	DrawDebugSphere(GetWorld(), ImpactLocation, JumpDownDamageRadius, 24, bHit ? FColor::Red : FColor::Green, false, 2.0f);
}
