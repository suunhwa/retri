// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/DashAbility.h"
#include "Player/Abilities/DashGhostActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

UDashAbility::UDashAbility()
{
	// 충전 시스템이 쿨타임을 직접 관리하므로 기반 쿨타임 비활성화
	Cooldown = 0.f;
}

bool UDashAbility::CanActivate(ACharacter* Owner)
{
	return CurrentCharges > 0;
}

void UDashAbility::Activate(ACharacter* Owner)
{
	// WASD 입력 방향 갱신 (마우스 방향 무시)
	FVector Dir = Owner->GetLastMovementInputVector();
	if (Dir.IsNearlyZero())
	{
		Dir = Owner->GetActorForwardVector();
	}
	Dir.Z = 0.f;
	Dir.Normalize();

	// 대시 방향으로 캐릭터 회전
	Owner->SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));

	Owner->LaunchCharacter(Dir * DashImpulse, true, false);

	// 충전 소모
	CurrentCharges--;
	OnChargeChanged.Broadcast(CurrentCharges, MaxDashCharges);

	// 개별 충전 회복 타이머
	FTimerHandle NewHandle;
	RechargeHandles.Add(NewHandle);
	Owner->GetWorldTimerManager().SetTimer(
		RechargeHandles.Last(),
		this, &UDashAbility::RestoreCharge,
		DashRechargeTime, false
	);

	if (DashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), DashSound, Owner->GetActorLocation());
	}

	if (DashCS)
	{
		if (APlayerController* PC = Cast<APlayerController>(Owner->GetController()))
		{
			PC->ClientStartCameraShake(DashCS);
		}
	}

	// 잔상 생성 시작
	if (GhostActorClass)
	{
		GhostSpawnedCount = 0;
		GhostOwner = Owner;
		Owner->GetWorldTimerManager().SetTimer(
			GhostSpawnTimer,
			this, &UDashAbility::SpawnGhost,
			GhostSpawnInterval, true
		);
	}
}

void UDashAbility::RestoreCharge()
{
	CurrentCharges = FMath::Min(CurrentCharges + 1, MaxDashCharges);
	OnChargeChanged.Broadcast(CurrentCharges, MaxDashCharges);
}

void UDashAbility::SpawnGhost()
{
	if (!GhostOwner.IsValid() || !GhostActorClass) return;

	ACharacter* Owner = GhostOwner.Get();
	USkeletalMeshComponent* Mesh = Owner->GetMesh();
	if (!Mesh) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADashGhostActor* Ghost = Owner->GetWorld()->SpawnActor<ADashGhostActor>(
		GhostActorClass,
		Mesh->GetComponentLocation(),
		Mesh->GetComponentRotation(),
		SpawnParams
	);

	if (Ghost)
	{
		Ghost->Init(Mesh);
	}

	GhostSpawnedCount++;
	if (GhostSpawnedCount >= GhostCount)
	{
		Owner->GetWorldTimerManager().ClearTimer(GhostSpawnTimer);
	}
}
