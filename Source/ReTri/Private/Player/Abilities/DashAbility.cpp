// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/DashAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}
