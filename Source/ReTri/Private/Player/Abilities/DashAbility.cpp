// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/DashAbility.h"
#include "Player/Abilities/DashGhostActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

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

	Owner->LaunchCharacter(Dir * DashImpulse, true, false);

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
