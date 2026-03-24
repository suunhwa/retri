// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/Skills/HandCannonSkill.h"

#include "GameFramework/Character.h"
#include "Player/Projectiles/HandCannonBullet.h"
#include "Player/ReTriPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"


UHandCannonSkill::UHandCannonSkill()
{
	Cooldown = 5.5f;
}

void UHandCannonSkill::Activate(ACharacter* Owner)
{
	AReTriPlayerController* pc = Cast<AReTriPlayerController>(Owner->GetController());
	if (!pc) return;
	
	FVector TargetPoint;
	if (!pc->GetMouseWorldPosition(TargetPoint)) return;
	
	FVector Direction = TargetPoint - Owner->GetActorLocation();
	Direction.Z = 0.0f;
	if (Direction.IsNearlyZero()) return;
	Direction.Normalize();
	
	// 발사 방향으로 캐릭터 회전
	Owner->SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));
	
	if (HandCannonClass)
	{
		FVector MuzzleLocation = Owner->GetMesh()->GetSocketLocation(TEXT("weapon_muzzle"));
		
		FActorSpawnParameters SpawnInfo; 
		SpawnInfo.Owner = Owner;
		SpawnInfo.Instigator = Owner;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		Owner->GetWorld()->SpawnActor<AHandCannonBullet>(HandCannonClass, MuzzleLocation, Direction.Rotation(), SpawnInfo);
	}
	
	// 플레이어 반동 (발사 반대 방향으로)
	Owner->LaunchCharacter(-Direction * RecoilForce, true, false);
	
	// effects
	// niagara
	if (HandCannonEffect)
	{
		FVector MuzzleLocation = Owner->GetMesh()->GetSocketLocation(TEXT("weapon_muzzle"));
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Owner->GetWorld(), HandCannonEffect, MuzzleLocation);
	}
	
	// particle
	if (MuzzleEffect)
	{
		FVector MuzzleLocation = Owner->GetMesh()->GetSocketLocation(TEXT("weapon_muzzle"));
		UGameplayStatics::SpawnEmitterAtLocation(Owner->GetWorld(), MuzzleEffect, MuzzleLocation);
	}
	
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), FireSound, Owner->GetActorLocation());
	}
}

