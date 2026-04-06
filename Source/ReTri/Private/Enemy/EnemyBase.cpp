// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyBase.h"

#include "AIController.h"
#include "AudioDevice.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Enemy/BP_StateTreeTask.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MapSubSystem.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// BoxDecalComp->SetupAttachment(RootComponent);
	//
	// BoxDecalComp->SetVisibility(false);
	// BoxDecalComp->SetComponentTickEnabled(false);
	// BoxDecalComp->SetRelativeLocation(FVector(0, 0, -1000.f));
	// GetMesh()->SetReceivesDecals(false);
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
	
	if (bIsJumpDownAttacking)
	{
		if (bIsEnhancedJump)
		{
			ExecuteEnhancedJumpDownDamage();
		}
		else
		{
			ExecuteJumpDownDamage();
		}

		// 상태 초기화
		bIsJumpDownAttacking = false;
		bIsEnhancedJump = false;
	}
	
	// 데칼 정리
	if (CircleDecalComp)
	{
		CircleDecalComp->DestroyComponent();
		CircleDecalComp = nullptr;
	}

	if (CrossDecalHorizontal)
	{
		CrossDecalHorizontal->DestroyComponent();
		CrossDecalHorizontal = nullptr;
	}

	if (CrossDecalVertical)
	{
		CrossDecalVertical->DestroyComponent();
		CrossDecalVertical = nullptr;
	}
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
	
	// === 증오 ===
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UMapSubSystem* MapSub = GI->GetSubsystem<UMapSubSystem>())
		{
			MapSub->UpdateCurseQuest(EActiveCurseQuest::KillMinions, 1);
		}
	}
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

// ---------------------------------------- 점프 다운
void AEnemyBase::SpawnJumpDecal(FVector Location, class UMaterialInterface* Decal)
{
	CircleDecalComp = UGameplayStatics::SpawnDecalAtLocation
		(
		GetWorld(),
		Decal,
		FVector(700.f,700.f,700.f),
		Location,
		FRotator(-90, 0, 0),
		2.0f
		);
	
	if (CircleDecalComp)
	{
		UMaterialInstanceDynamic* LocalDynamicDecal = CircleDecalComp->CreateDynamicMaterialInstance();
		
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

// ---------------------------------------- 분신 검기
void AEnemyBase::SpawnClones()
{
	for (AEnemyBase* Clone : ActiveClones)
	{
		if (Clone)
		{
			Clone->Destroy();
		}
	}
	ActiveClones.Empty();

	const int32 CloneCount = 7;
	const float Radius = 1800.f;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player || !CloneClass) return;

	for (int32 i = 0; i < CloneCount; ++i)
	{
		const float AngleDeg = (360.f / CloneCount) * i;
		const float AngleRad = FMath::DegreesToRadians(AngleDeg);

		FVector Offset(
			FMath::Cos(AngleRad),
			FMath::Sin(AngleRad),
			0.f
		);
		Offset *= Radius;

		const FVector SpawnLoc = GetActorLocation() + Offset;

		// 각 분신 위치 기준으로 플레이어를 바라보게
		FVector Dir = Player->GetActorLocation() - SpawnLoc;
		Dir.Z = 0.f;

		if (Dir.IsNearlyZero())
		{
			Dir = GetActorForwardVector();
			Dir.Z = 0.f;
		}

		Dir.Normalize();
		const FRotator TargetRot = Dir.Rotation();

		AEnemyBase* NewClone = GetWorld()->SpawnActor<AEnemyBase>(
			CloneClass,
			SpawnLoc,
			TargetRot
		);

		if (!NewClone) continue;

		NewClone->SetActorRotation(TargetRot);
		NewClone->bCanRotate = false;
		NewClone->PrimaryActorTick.bCanEverTick = false;

		if (NewClone->GetCharacterMovement())
		{
			NewClone->GetCharacterMovement()->bOrientRotationToMovement = false;
			NewClone->GetCharacterMovement()->RotationRate = FRotator::ZeroRotator;
		}

		if (NewClone->GetController())
		{
			NewClone->GetController()->SetControlRotation(TargetRot);
		}

		ActiveClones.Add(NewClone);
	}
}

void AEnemyBase::InitCloneDecal(float Width, float Length)
{
	if (!BoxDecal) return;

	// 기존 데칼 제거
	if (ActiveDecal)
	{
		ActiveDecal->DestroyComponent();
		ActiveDecal = nullptr;
		DynamicBoxDecal = nullptr;
	}

	const float Thickness = 300.f;

	FVector Forward = GetActorForwardVector();
	FRotator Rot = Forward.Rotation();

	FVector Location =
		GetActorLocation() +
		(Forward * (Length * 0.5f));

	Location.Z -= 100.f;

	FRotator FinalRot(-90.f, Rot.Yaw , 0.f);

	ActiveDecal = UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		BoxDecal,
		FVector(Thickness, Width, Length),
		Location,
		FinalRot,
		5.0f
	);

	if (ActiveDecal)
	{
		DynamicBoxDecal = ActiveDecal->CreateDynamicMaterialInstance();

		if (DynamicBoxDecal)
		{
			DynamicBoxDecal->SetScalarParameterValue(TEXT("BoxProgress"), 0.0f);
		}
	}
}

// ---------------------------------------- 분신검기 데칼
void AEnemyBase::StartDecalProgress(float Duration)
{
	if (!DynamicBoxDecal) return;

	TSharedPtr<FTimerHandle> TimerHandlePtr = MakeShared<FTimerHandle>();
	float UpdateInterval = 0.02f;

	float* Elapsed = new float(0.0f);

	GetWorldTimerManager().SetTimer(*TimerHandlePtr,
		[this, Duration, TimerHandlePtr, Elapsed]() mutable
		{
			if (!DynamicBoxDecal)
			{
				GetWorldTimerManager().ClearTimer(*TimerHandlePtr);
				delete Elapsed;
				return;
			}

			*Elapsed += 0.02f;

			float Progress = FMath::Clamp(*Elapsed / Duration, 0.0f, 1.0f);

			DynamicBoxDecal->SetScalarParameterValue("BoxProgress", Progress);

			if (Progress >= 1.0f)
			{
				GetWorldTimerManager().ClearTimer(*TimerHandlePtr);
				delete Elapsed;
			}
		},
		UpdateInterval,
		true
	);
}

// ---------------------------------------- 점프다운 대미지
void AEnemyBase::ExecuteJumpDownDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("ExecuteJumpDownDamage Called"));
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

// ---------------------------------------- 분신검기 대미지
void AEnemyBase::ExecuteMirrorBladeDamage(AEnemyBase* Clone)
{
	if (!Clone) return;

	UWorld* World = GetWorld();
	if (!World) return;

	float Width = 110.f;
	float Length = 2400.f;
	float Height = 100.f;

	FVector Forward = Clone->GetActorForwardVector();

	FVector BaseLocation =
		Clone->GetActorLocation() +
		(Forward * (Length * 0.5f));

	BaseLocation.Z -= 100.f;

	FVector BoxCenter = BaseLocation;
	BoxCenter.Z += Height * 0.5f;

	FVector BoxExtent = FVector(
		Length * 0.5f,
		Width * 0.5f,
		Height * 0.5f
	);

	TArray<AActor*> OverlappedActors;
	TArray<AActor*> IgnoreActors;

	IgnoreActors.Add(this);

	for (AEnemyBase* C : ActiveClones)
	{
		if (C) IgnoreActors.Add(C);
	}

	// TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel5));

	bool bHit = UKismetSystemLibrary::BoxOverlapActors(
		World,
		BoxCenter,
		BoxExtent,
		{},
		ACharacter::StaticClass(),
		IgnoreActors,
		OverlappedActors
	);

	if (bHit)
	{
		for (AActor* Actor : OverlappedActors)
		{
			ACharacter* Player = Cast<ACharacter>(Actor);
			if (!Player) continue;
			
			if (Player->IsPlayerControlled())
			{
				UGameplayStatics::ApplyDamage(
					Actor,
					CurrentSkillDamage,
					GetController(),
					this,
					nullptr
				);
			}
		}
	}

	DrawDebugBox(
		World,
		BoxCenter,
		BoxExtent,
		FRotator(0.f, Clone->GetActorRotation().Yaw, 0.f).Quaternion(),
		FColor::Red,
		false,
		1.0f,
		0,
		2.0f
	);
}

// ---------------------------------------- 강화 대쉬
void AEnemyBase::ExecuteReinforcedDash(FVector StartLoc, FRotator DashRot)
{
	UWorld* World = GetWorld();
	if (!World) return;

	int32 NumberOfClones = 1;      // 소환할 분신 개수
	float DistanceBetween = 400.f; // 분신 간의 간격
	float TimeDelay = 0.1f;        // 분신이 나타나는 시간차

	for (int32 i = 1; i <= NumberOfClones; i++)
	{
		// 소환될 위치 계산 (보스가 바라보는 방향으로 순차적 배치)
		FVector SpawnLocation = StartLoc + (DashRot.Vector() * (DistanceBetween * i));
        
		// 시간차를 두고 분신 소환 (람다 캡처 주의)
		FTimerHandle CloneTimer;
		GetWorldTimerManager().SetTimer(CloneTimer, [this, World, SpawnLocation, DashRot]()
		{
			AEnemyBase* NewClone = World->SpawnActor<AEnemyBase>(CloneClass, SpawnLocation, DashRot);
            
			if (NewClone)
			{
				// 분신 설정
				NewClone->bIsBoss = false;
                
				// 공격 애니메이션 실행
				if (CloneAttackMontage)
				{
					NewClone->PlayAnimMontage(CloneAttackMontage);
				}

				// 분신이 공격 후 자연스럽게 사라지도록 설정
				NewClone->SetLifeSpan(1.2f); 
			}
		}, i * TimeDelay, false);
	}
}


// ---------------------------------------- 강화 점프
void AEnemyBase::SpawnEnhancedJumpDecal(FVector Location, class UMaterialInterface* CircleDecal,
	class UMaterialInterface* CrossDecal)
{
	CircleDecalComp = UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		CircleDecal,
		FVector(700.f, 700.f, 700.f),
		Location,
		FRotator(-90.f, 0.f, 0.f),
		2.0f
	);

	CrossDecalHorizontal = UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		CrossDecal,
		FVector(250.f, 200.f, 3200.f),
		Location,
		FRotator(-90.f, 0.f, 0.f),
		2.0f
	);

	CrossDecalVertical = UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		CrossDecal,
		FVector(250.f, 200.f, 3200.f),
		Location,
		FRotator(-90.f, 90.f, 0.f),
		2.0f
	);

	if (CrossDecalHorizontal)
	{
		CrossDecalHorizontal->SetVisibility(false);
	}

	if (CrossDecalVertical)
	{
		CrossDecalVertical->SetVisibility(false);
	}

	UMaterialInstanceDynamic* CircleMID = nullptr;

	if (CircleDecalComp)
	{
		CircleMID = CircleDecalComp->CreateDynamicMaterialInstance();
	}

	if (CircleMID)
	{
		float UpdateInterval = 0.02f;

		TSharedPtr<FTimerHandle> TimerHandlePtr = MakeShared<FTimerHandle>();
		TWeakObjectPtr<AEnemyBase> WeakThis = this;
		UWorld* WorldContext = GetWorld();

		WorldContext->GetTimerManager().SetTimer(
			*TimerHandlePtr,
			[WeakThis, WorldContext, CircleMID, TimerHandlePtr, Location, Progress = 0.0f]() mutable
			{
				if (!WeakThis.IsValid())
				{
					if (IsValid(WorldContext))
					{
						WorldContext->GetTimerManager().ClearTimer(*TimerHandlePtr);
					}
					return;
				}

				AEnemyBase* Boss = WeakThis.Get();
				if (!Boss) return;

				if (!IsValid(CircleMID))
				{
					Boss->GetWorldTimerManager().ClearTimer(*TimerHandlePtr);
					return;
				}

				Progress += (0.5f / (1.0f / 0.02f));
				float ClampedProgress = FMath::Min(Progress, 0.5f);

				CircleMID->SetScalarParameterValue(TEXT("Progress Circle"), ClampedProgress);

				// 막판에만 십자 표시
				if (Progress >= 0.42f)
				{
					if (Boss->CrossDecalHorizontal)
					{
						Boss->CrossDecalHorizontal->SetVisibility(true);
					}

					if (Boss->CrossDecalVertical)
					{
						Boss->CrossDecalVertical->SetVisibility(true);
					}
				}

				if (Progress >= 0.5f)
				{
					Boss->GetWorldTimerManager().ClearTimer(*TimerHandlePtr);

					FVector DownStartLoc = Location + FVector(0.0f, 0.0f, 1500.0f);
					Boss->SetActorLocation(DownStartLoc, false);

					Boss->SetActorHiddenInGame(false);
					Boss->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					Boss->GetCharacterMovement()->SetMovementMode(MOVE_Falling);

					Boss->LaunchCharacter(FVector(0, 0, -5000.f), true, true);
					Boss->PlayAnimMontage(Boss->DownMontage, 1.0f);
				}
			},
			UpdateInterval,
			true
		);
	}
}

// ---------------------------------------- 강화 점프다운 원형 대미지
void AEnemyBase::ExecuteEnhancedJumpDownDamage()
{
	// UE_LOG(LogTemp, Warning, TEXT("ExecuteEnhancedJumpDownDamage Called"));
	UWorld* World = GetWorld();
	if (!World) return;

	FVector ImpactLocation = GetActorLocation();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	// 원형 대미지
	UGameplayStatics::ApplyRadialDamage(
		World,
		CurrentSkillDamage,
		ImpactLocation,
		JumpDownDamageRadius,
		nullptr,
		IgnoreActors,
		this,
		GetInstigatorController(),
		false
	);

	// 십자 대미지 호출
	ExecuteJumpCrossDamage(ImpactLocation);

	DrawDebugSphere(
		World,
		ImpactLocation,
		JumpDownDamageRadius,
		24,
		FColor::Red,
		false,
		2.0f
	);
}

// ---------------------------------------- 강화 점프다운 십자 대미지
void AEnemyBase::ExecuteJumpCrossDamage(FVector ImpactLocation)
{
	UWorld* World = GetWorld();
	if (!World) return;

	const float CrossLength = 3200.f;
	const float CrossThickness = 200.f;
	const float Height = 250.f;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<AActor*> OverlappedActors;
	TSet<AActor*> HitActors;

	// 가로
	UKismetSystemLibrary::BoxOverlapActors(
		World,
		ImpactLocation,
		FVector(CrossLength * 0.5f, CrossThickness * 0.5f, Height * 0.5f),
		{},
		ACharacter::StaticClass(),
		IgnoreActors,
		OverlappedActors
	);

	for (AActor* Actor : OverlappedActors)
	{
		if (ACharacter* Player = Cast<ACharacter>(Actor))
		{
			if (Player->IsPlayerControlled())
			{
				HitActors.Add(Player);
			}
		}
	}

	OverlappedActors.Empty();

	// 세로
	UKismetSystemLibrary::BoxOverlapActors(
		World,
		ImpactLocation,
		FVector(CrossThickness * 0.5f, CrossLength * 0.5f, Height * 0.5f),
		{},
		ACharacter::StaticClass(),
		IgnoreActors,
		OverlappedActors
	);

	for (AActor* Actor : OverlappedActors)
	{
		if (ACharacter* Player = Cast<ACharacter>(Actor))
		{
			if (Player->IsPlayerControlled())
			{
				HitActors.Add(Player);
			}
		}
	}

	for (AActor* Actor : HitActors)
	{
		UGameplayStatics::ApplyDamage(
			Actor,
			CurrentSkillDamage,
			GetInstigatorController(),
			this,
			nullptr
		);
	}

	DrawDebugBox(
		World,
		ImpactLocation,
		FVector(CrossLength * 0.5f, CrossThickness * 0.5f, Height * 0.5f),
		FColor::Blue,
		false,
		2.0f,
		0,
		3.0f
	);

	DrawDebugBox(
		World,
		ImpactLocation,
		FVector(CrossThickness * 0.5f, CrossLength * 0.5f, Height * 0.5f),
		FColor::Blue,
		false,
		2.0f,
		0,
		3.0f
	);
}
