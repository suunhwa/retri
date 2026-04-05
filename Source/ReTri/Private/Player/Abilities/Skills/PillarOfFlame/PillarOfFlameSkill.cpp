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
#include "Player/Abilities/Skills/PillarOfFlame/FireOrbProjectile.h"

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

	// 손에서 타겟 방향으로 Orb 발사
	if (FireOrbClass)
	{
		FVector MuzzleLocation = Owner->GetMesh()->GetSocketLocation(TEXT("hand_r"));
		UE_LOG(LogTemp, Warning, TEXT("[PillarOfFlame] Orb 스폰 위치: %s"), *MuzzleLocation.ToString());
		FVector Direction = TargetPoint - MuzzleLocation;
		Direction.Z = 0.f;
		Direction.Normalize();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Owner;
		SpawnParams.Instigator = Owner;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AFireOrbProjectile* Orb = Owner->GetWorld()->SpawnActor<AFireOrbProjectile>(
			FireOrbClass, MuzzleLocation, Direction.Rotation(), SpawnParams
		);

		if (Orb)
		{
			Orb->Init(AP, Owner->GetController(), PillarAoEClass);
		}
	}

	// 타겟 위치 이펙트
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
