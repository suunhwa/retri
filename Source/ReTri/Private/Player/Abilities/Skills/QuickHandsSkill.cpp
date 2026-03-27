// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/Skills/QuickHandsSkill.h"

#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Player/ReTriPlayerController.h"
#include "Player/Components/StatComponent.h"
#include "Player/Projectiles/PiercingBullet.h"
#include "TimerManager.h"

UQuickHandsSkill::UQuickHandsSkill()
{
	Cooldown = 0.1f;
}

bool UQuickHandsSkill::CanActivate(ACharacter* Owner)
{
	return CurrentCharges > 0;
}

void UQuickHandsSkill::Activate(ACharacter* Owner)
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
		this, &UQuickHandsSkill::RestoreCharge,
		RechargeTime, false
	);
	
	// 공격력 가져오기
	float BaseDamage = 10.f;
	if (UStatComponent* StatComp = Player->GetStatComponent())
	{
		BaseDamage = StatComp->GetAttackDamage();
	}
	
	// 공격 속도로 발사 간격 계산
	float AttackSpeed = 1.f;
	if (UStatComponent* StatComp = Player->GetStatComponent())
	{
		AttackSpeed = FMath::Max(0.1f, StatComp->GetAttackSpeed());
	}
	
	const float FireDelay = BaseFireDelay / AttackSpeed;
	
	TWeakObjectPtr<ACharacter> WeakOwner(Owner);
	
	// 1번탄 즉시 발사 (120%)
	FireShot(WeakOwner, 0, Direction, BaseDamage);
	
	// 2번탄 즉시 발사 (150%)
	FTimerDelegate Del2;
	Del2.BindUObject(this, &UQuickHandsSkill::FireShot, WeakOwner, 1, Direction, BaseDamage);
	Owner->GetWorldTimerManager().SetTimer(FireTimerHandle2, Del2, FireDelay, false);
	
	// 3번탄 즉시 발사 (420%)
	FTimerDelegate Del3;
	Del3.BindUObject(this, &UQuickHandsSkill::FireShot, WeakOwner, 2, Direction, BaseDamage);
	Owner->GetWorldTimerManager().SetTimer(FireTimerHandle3, Del3, FireDelay * 2.f, false);
	
	// effects
	if (FireEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner->GetWorld(), FireEffect, Owner->GetActorLocation());
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), FireSound, Owner->GetActorLocation());
	}
}

void UQuickHandsSkill::FireShot(TWeakObjectPtr<ACharacter> WeakOwner,
	int32 ShotIndex,
	FVector Direction,
	float BaseDamage)
{
	if (!WeakOwner.IsValid() || !PiercingBullet) return;
	ACharacter* Owner = WeakOwner.Get();
	
	// 인덱스 방어
	if (!DamageMultipliers.IsValidIndex(ShotIndex)) return;
	
	FVector MuzzleLocation = Owner->GetMesh()->GetSocketLocation(TEXT("weapon_muzzle"));

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
		Bullet->SetDamage(BaseDamage * DamageMultipliers[ShotIndex]);
	}
}

void UQuickHandsSkill::RestoreCharge()
{
	CurrentCharges = FMath::Min(CurrentCharges + 1, MaxCharges);
}
