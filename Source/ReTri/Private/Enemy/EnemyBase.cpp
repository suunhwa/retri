// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyBase.h"
#include <Codecapi.h>

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
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
