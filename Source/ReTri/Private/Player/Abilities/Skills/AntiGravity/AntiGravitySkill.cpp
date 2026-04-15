#include "Player/Abilities/Skills/AntiGravity/AntiGravitySkill.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Player/PlayerCharacter.h"
#include "Player/ReTriPlayerController.h"
#include "Player/Components/StatComponent.h"
#include "Player/Abilities/Skills/AntiGravity/AntiGravityAoE.h"

UAntiGravitySkill::UAntiGravitySkill()
{
	Cooldown = 14.f;
}

void UAntiGravitySkill::Activate(ACharacter* Owner)
{
	AReTriPlayerController* PC = Cast<AReTriPlayerController>(Owner->GetController());
	if (!PC) return;

	FVector TargetPoint;
	if (!PC->GetMouseWorldPosition(TargetPoint)) return;

	if (!AntiGravityAoEClass) return;

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

	AAntiGravityAoE* AntiGravity = Owner->GetWorld()->SpawnActorDeferred<AAntiGravityAoE>(
		AntiGravityAoEClass,
		SpawnTransform,
		Owner,
		Owner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (AntiGravity)
	{
		AntiGravity->Init(AP, Owner->GetController());
		UGameplayStatics::FinishSpawningActor(AntiGravity, SpawnTransform);
	}

	if (CastEffect)
	{
		if (UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			Owner->GetWorld(),
			CastEffect,
			TargetPoint))
		{
			NC->SetAutoDestroy(true);
			FTimerHandle TimerHandle;
			TWeakObjectPtr<UNiagaraComponent> WeakNC = NC;
			Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle,
			                                              [WeakNC]()
			                                              {
				                                              if (WeakNC.IsValid()) WeakNC->DeactivateImmediate();
			                                              },
			                                              EffectDuration,
			                                              false);
		}
	}

	if (CastSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), CastSound, TargetPoint);
	}
}
