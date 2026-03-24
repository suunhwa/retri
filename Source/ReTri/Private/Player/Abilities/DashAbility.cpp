// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDashAbility::Activate(ACharacter* Owner)
{
	// 현재 이동 입력 방향으로 대시, 없으면 캐릭터 전방
	FVector Dir = Owner->GetLastMovementInputVector();
	if (Dir.IsNearlyZero())
	{
		Dir = Owner->GetActorForwardVector();
	}
	Dir.Z = 0.f;
	Dir.Normalize();

	Owner->LaunchCharacter(Dir * DashImpulse, true, false);
}
