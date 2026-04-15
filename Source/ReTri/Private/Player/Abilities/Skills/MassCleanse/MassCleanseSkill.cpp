#include "Player/Abilities/Skills/MassCleanse/MassCleanseSkill.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Player/PlayerCharacter.h"
#include "Player/Components/HealthComponent.h"
#include "Player/Components/StatComponent.h"
#include "Player/Abilities/Skills/MassCleanse/MassCleanseAoE.h"

UMassCleanseSkill::UMassCleanseSkill()
{
	Cooldown = 10.f;
}

void UMassCleanseSkill::Activate(ACharacter* Owner)
{
	if (!CleanseAoEClass) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	if (!Player) return;

	float AP = Player->GetStatComponent()->GetSpellPower();

	// 플레이어 위치에 AoE 소환
	const FTransform SpawnTransform(FRotator::ZeroRotator, Owner->GetActorLocation());

	AMassCleanseAoE* Cleanse = Owner->GetWorld()->SpawnActorDeferred<AMassCleanseAoE>(
		CleanseAoEClass,
		SpawnTransform,
		Owner,
		Owner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (Cleanse)
	{
		Cleanse->Init(AP, Owner->GetController());
		UGameplayStatics::FinishSpawningActor(Cleanse, SpawnTransform);
	}

	// 플레이어 체력 회복
	Player->GetHealthComponent()->Heal(HealAmount);

	// 플레이어 몸에 붙는 힐 이펙트
	if (HealEffect)
	{
		UNiagaraComponent* HealNC = UNiagaraFunctionLibrary::SpawnSystemAttached(
			HealEffect,
			Owner->GetMesh(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		if (HealNC)
		{
			FTimerHandle HealFXTimer;
			Owner->GetWorldTimerManager().SetTimer(HealFXTimer,
			                                       [HealNC]()
			                                       {
				                                       if (HealNC) HealNC->DeactivateImmediate();
			                                       },
			                                       CastEffectDuration,
			                                       false);
		}
	}

	if (CastEffect)
	{
		if (UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			Owner->GetWorld(),
			CastEffect,
			Owner->GetActorLocation()))
		{
			NC->SetAutoDestroy(true);
			FTimerHandle TimerHandle;
			Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle,
			                                              [NC]()
			                                              {
				                                              if (NC) NC->DeactivateImmediate();
			                                              },
			                                              CastEffectDuration,
			                                              false);
		}
	}

	if (CastSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), CastSound, Owner->GetActorLocation());
	}
}
