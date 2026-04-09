// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/Skills/PillarOfFlame/PillarOfFlameSkill.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Player/PlayerCharacter.h"
#include "Player/ReTriPlayerController.h"
#include "Player/Components/StatComponent.h"
#include "Player/Abilities/Skills/PillarOfFlame/PillarOfFlameAoE.h"

UPillarOfFlameSkill::UPillarOfFlameSkill()
{
	Cooldown = 11.f;
}

void UPillarOfFlameSkill::Activate(ACharacter* Owner)
{
	AReTriPlayerController* PC = Cast<AReTriPlayerController>(Owner->GetController());
	if (!PC) return;

	FVector TargetPoint;
	if (!PC->GetMouseWorldPosition(TargetPoint)) return;

	float AP = 0.f;
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Owner))
	{
		AP = Player->GetStatComponent()->GetSpellPower();
	}

	if (!PillarAoEClass) return;

	const FTransform SpawnTransform(FRotator::ZeroRotator, TargetPoint);
	APillarOfFlameAoE* Pillar = Owner->GetWorld()->SpawnActorDeferred<APillarOfFlameAoE>(
		PillarAoEClass, SpawnTransform, Owner, Owner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	if (Pillar)
	{
		Pillar->Init(AP, Owner->GetController());
		UGameplayStatics::FinishSpawningActor(Pillar, SpawnTransform);
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
