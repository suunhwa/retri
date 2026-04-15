#include "Player/Abilities/Skills/QuickTriggerSkill.h"

#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Player/PlayerCharacter.h"
#include "Player/ReTriPlayerController.h"
#include "Player/Components/StatComponent.h"
#include "Player/Projectiles/PiercingBullet.h"
#include "TimerManager.h"

UQuickTriggerSkill::UQuickTriggerSkill()
{
	Cooldown = 0.1f;
}

bool UQuickTriggerSkill::CanActivate(ACharacter* Owner)
{
	return CurrentCharges > 0;
}

void UQuickTriggerSkill::Activate(ACharacter* Owner)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	if (!Player) return;

	AReTriPlayerController* pc = Cast<AReTriPlayerController>(Owner->GetController());
	if (!pc) return;

	FVector TargetPoint;
	if (!pc->GetMouseWorldPosition(TargetPoint)) return;

	FVector Direction = TargetPoint - Owner->GetActorLocation();
	Direction.Z = 0.0f;
	if (Direction.IsNearlyZero()) return;
	Direction.Normalize();

	// 발사 방향으로 캐릭터 회전
	Owner->SetActorRotation(FRotator(0, Direction.Rotation().Yaw, 0));

	// 충전 소모 
	CurrentCharges--;

	FTimerHandle NewHandle;
	RechargeTimerHandle.Add(NewHandle);
	Owner->GetWorldTimerManager().SetTimer(
		RechargeTimerHandle.Last(),
		this,
		&UQuickTriggerSkill::RestoreCharge,
		RechargeTime,
		false
	);

	// 공격력 가져오기
	float AttackDamage = 10.f;
	if (UStatComponent* StatComp = Player->GetStatComponent())
	{
		AttackDamage = StatComp->GetAttackPower();
	}

	// 공격 속도로 발사 간격 계산
	float AttackSpeed = 1.f;
	if (UStatComponent* StatComp = Player->GetStatComponent())
	{
		AttackSpeed = FMath::Max(0.1f, StatComp->GetAttackSpeed());
	}

	const float FireDelay = BaseFireDelay / AttackSpeed;

	// Activate 시점에 캡처 — 타이머 딜레이 동안 애니메이션으로 소켓 위치가 바뀌어도 동일한 위치에서 발사
	FVector MuzzleLocation = Owner->GetMesh()->GetSocketLocation(TEXT("weapon_muzzle"));

	TWeakObjectPtr<ACharacter> WeakOwner(Owner);

	// 1번탄 즉시 발사 (120%)
	FireShot(WeakOwner, 0, Direction, AttackDamage, MuzzleLocation);

	// 2번탄 (150%)
	FTimerDelegate Del2;
	Del2.BindUObject(this, &UQuickTriggerSkill::FireShot, WeakOwner, 1, Direction, AttackDamage, MuzzleLocation);
	Owner->GetWorldTimerManager().SetTimer(FireTimerHandle2, Del2, FireDelay, false);

	// 3번탄 (420%)
	FTimerDelegate Del3;
	Del3.BindUObject(this, &UQuickTriggerSkill::FireShot, WeakOwner, 2, Direction, AttackDamage, MuzzleLocation);
	Owner->GetWorldTimerManager().SetTimer(FireTimerHandle3, Del3, FireDelay * 2.f, false);

	// 사운드는 첫 발에만 (3발 모두 틀면 너무 시끄러움)
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), FireSound, Owner->GetActorLocation());
	}

	// 셰이크도 발동 시 1회
	if (FireCS)
	{
		if (APlayerController* PC = Cast<APlayerController>(Owner->GetController()))
		{
			PC->ClientStartCameraShake(FireCS);
		}
	}
}

void UQuickTriggerSkill::FireShot(TWeakObjectPtr<ACharacter> WeakOwner,
                                  int32 ShotIndex,
                                  FVector Direction,
                                  float AttackDamage,
                                  FVector MuzzleLocation)
{
	if (!WeakOwner.IsValid() || !PiercingBullet) return;
	ACharacter* Owner = WeakOwner.Get();

	if (!DamageMultipliers.IsValidIndex(ShotIndex)) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.Instigator = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APiercingBullet* Bullet = GetWorld()->SpawnActor<APiercingBullet>(
		PiercingBullet,
		MuzzleLocation,
		Direction.Rotation(),
		SpawnParams
	);

	if (Bullet)
	{
		Bullet->SetDamage(AttackDamage * DamageMultipliers[ShotIndex]);
	}

	// 발당 머즐 이펙트
	if (FireEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEffect, MuzzleLocation);
	}

	if (MuzzleParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticleEffect, MuzzleLocation);
	}
}

void UQuickTriggerSkill::RestoreCharge()
{
	CurrentCharges = FMath::Min(CurrentCharges + 1, MaxCharges);
}
