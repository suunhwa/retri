// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/Skills/CoinExplosion/CoinExplosionSkill.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Player/PlayerCharacter.h"
#include "Player/ReTriPlayerController.h"
#include "Player/Components/StatComponent.h"
#include "Player/Projectiles/CoinProjectile.h"

UCoinExplosionSkill::UCoinExplosionSkill()
{
	Cooldown = 5.f;
}

bool UCoinExplosionSkill::CanActivate(ACharacter* Owner)
{
	if (!Owner) return false;

	APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	if (!Player) return false;

	UStatComponent* StatComp = Player->GetStatComponent();
	if (!StatComp) return false;

	// 던질 동전이 없으면 사용 불가
	return StatComp->GetGold() > 0;
}

void UCoinExplosionSkill::Activate(ACharacter* Owner)
{
	if (!Owner) return;
	if (!CoinProjectileClass) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	if (!Player) return;

	UStatComponent* StatComp = Player->GetStatComponent();
	if (!StatComp) return;

	AReTriPlayerController* PC = Cast<AReTriPlayerController>(Owner->GetController());
	if (!PC) return;

	// 현재 골드의 1% (최소 1)
	const int32 CurrentGold = StatComp->GetGold();
	const int32 GoldCost = FMath::Max(1, FMath::CeilToInt(CurrentGold * 0.01f));

	if (!StatComp->SpendGold(GoldCost)) return;

	// 던지는 발사음
	if (CastSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), CastSound, Owner->GetActorLocation());
	}

	// 피해량 = 소모 골드 × 현재 배율
	const float FinalDamage = GoldCost * ExplosionDamageCoeff;

	FVector MouseWorldPos;
	if (!PC->GetMouseWorldPosition(MouseWorldPos)) return;

	const FVector OwnerLoc = Owner->GetActorLocation();
	const FVector LaunchDir = (MouseWorldPos - OwnerLoc).GetSafeNormal2D();

	const FTransform SpawnTransform(LaunchDir.Rotation(), OwnerLoc);

	ACoinProjectile* Coin = Owner->GetWorld()->SpawnActorDeferred<ACoinProjectile>(
		CoinProjectileClass,
		SpawnTransform,
		Owner,
		Owner,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (Coin)
	{
		// 폭발 완료 콜백 등록
		FOnCoinExploded Callback;
		Callback.BindUObject(this, &UCoinExplosionSkill::RollExplosionMultiplier);

		Coin->Init(FinalDamage, Owner->GetController(), Callback);
		UGameplayStatics::FinishSpawningActor(Coin, SpawnTransform);
	}
}

void UCoinExplosionSkill::RollExplosionMultiplier()
{
	// 50% 확률로 배율 2배, 50% 확률로 기본값 리셋
	if (FMath::RandBool())
	{
		ExplosionDamageCoeff *= 2.f;
	}
	else
	{
		ExplosionDamageCoeff = BaseExplosionDamageCoeff;
	}
}
