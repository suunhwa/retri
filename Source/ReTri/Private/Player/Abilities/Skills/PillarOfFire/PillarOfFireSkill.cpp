// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/Skills/PillarOfFire/PillarOfFireSkill.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Player/PlayerCharacter.h"
#include "Player/ReTriPlayerController.h"
#include "Player/Components/StatComponent.h"
#include "Player/Abilities/Skills/PillarOfFire/PillarOfFireAoE.h"

UPillarOfFireSkill::UPillarOfFireSkill()
{
	Cooldown = 11.f;
}

void UPillarOfFireSkill::Activate(ACharacter* Owner)
{
	AReTriPlayerController* PC = Cast<AReTriPlayerController>(Owner->GetController());
	if (!PC) return;

	FVector TargetPoint;
	if (!PC->GetMouseWorldPosition(TargetPoint)) return;

	if (!PillarAoEClass) return;

	float AP = 0.f;
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Owner))
	{
		AP = Player->GetStatComponent()->GetSpellPower();
	}

	// Init → BeginPlay 순서 보장을 위해 Deferred Spawn 사용
	const FTransform SpawnTransform(
		FRotator::ZeroRotator,
		FVector(TargetPoint.X, TargetPoint.Y, Owner->GetActorLocation().Z)
	);

	APillarOfFireAoE* Pillar = Owner->GetWorld()->SpawnActorDeferred<APillarOfFireAoE>(
		PillarAoEClass,
		SpawnTransform,
		Owner,
		Owner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (Pillar)
	{
		Pillar->Init(AP, Owner->GetController());
		UGameplayStatics::FinishSpawningActor(Pillar, SpawnTransform);
	}

	if (CastEffect)
	{
		if (UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner->GetWorld(), CastEffect, TargetPoint))
		{
			NC->SetAutoDestroy(true);
			FTimerHandle TimerHandle;
			Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NC]()
			{
				if (NC) NC->DeactivateImmediate();
			}, EffectDuration, false);
		}
	}

	if (CastSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), CastSound, TargetPoint);
	}
}
