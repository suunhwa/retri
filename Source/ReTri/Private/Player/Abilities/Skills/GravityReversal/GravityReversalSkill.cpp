// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/Skills/GravityReversal/GravityReversalSkill.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Player/ReTriPlayerController.h"
#include "Player/Components/StatComponent.h"
#include "Player/Abilities/Skills/GravityReversal/GravityReversalAoE.h"

UGravityReversalSkill::UGravityReversalSkill()
{
	Cooldown = 14.f;
}

void UGravityReversalSkill::Activate(ACharacter* Owner)
{
	AReTriPlayerController* PC = Cast<AReTriPlayerController>(Owner->GetController());
	if (!PC) return;

	FVector TargetPoint;
	if (!PC->GetMouseWorldPosition(TargetPoint)) return;

	if (!GravityReversalAoEClass) return;

	float AP = 0.f;
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Owner))
	{
		AP = Player->GetStatComponent()->GetAbilityPower();
	}

	// Init → BeginPlay 순서 보장을 위해 Deferred Spawn 사용
	const FTransform SpawnTransform(
		FRotator::ZeroRotator,
		FVector(TargetPoint.X, TargetPoint.Y, Owner->GetActorLocation().Z)
	);

	AGravityReversalAoE* GravityReversal = Owner->GetWorld()->SpawnActorDeferred<AGravityReversalAoE>(
		GravityReversalAoEClass,
		SpawnTransform,
		Owner,
		Owner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (GravityReversal)
	{
		GravityReversal->Init(AP, Owner->GetController());
		UGameplayStatics::FinishSpawningActor(GravityReversal, SpawnTransform);
	}

	if (CastEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner->GetWorld(), CastEffect, TargetPoint);
	}

	if (CastSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), CastSound, TargetPoint);
	}
}
