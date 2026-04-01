// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Minion/Minion.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/Minion/MyMinionFSM.h"


AMinion::AMinion()
{
	FSM = CreateDefaultSubobject<UMyMinionFSM>(TEXT("FSM"));
}

void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
	FSM  = Cast<UMyMinionFSM>(GetComponentByClass(UMyMinionFSM::StaticClass()));
}

void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// if (PlayerTarget)
	// {
	// 	CheckDistanceAndMove(DeltaTime);
	// }
}

void AMinion::CheckDistanceAndMove(float DeltaTime)
{
	// float Distance = GetDistanceTo(PlayerTarget);
	//
 //    
	// if (Distance > 300.0f) // 추격 범위
	// {
	// 	// 플레이어 향해 회전
	// 	RotateToTarget(DeltaTime, 5.0f);
	//
	// 	// 이동
	// 	if (AAIController* AIC = Cast<AAIController>(GetController()))
	// 	{
	// 		AIC->MoveToActor(PlayerTarget, 120.0f);
	// 	}
	// }
	// else // 공격 범위
	// {
	// 	if (!bIsAttacking && MyStatInfo && MyStatInfo->MinionAttackMontage)
	// 	{
	// 		bIsAttacking = true;
	// 		PlayAnimMontage(MyStatInfo->MinionAttackMontage);
 //            
	// 		// 몽타주 종료 후 bIsAttacking을 false로 돌리는 로직 필요 (애님노티파이)
	// 	}
	// }
}

float AMinion::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	// if (!IsValid(this)) return 0.0f;
	//
	// float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//
	// if (FSM)
	// {
	// 	if (CurrentHP - DamageAmount <= 0.0f)
	// 		FSM->_State = EMinionState::Die;
	// 	else
	// 		FSM->_State = EMinionState::Damage;
	// }
	// else 
	// {
	// 	CurrentHP -= DamageAmount;
	// }
	//
	// return ActualDamage;
	//
	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (ActualDamage > 0.0f && FSM != nullptr)
	{
		FlashRed();
		
		if (CurrentHP <= 0.0f)
		{
			FSM->_State = EMinionState::Die;
		}
		else
		{
			FSM->_State = EMinionState::Attack;	// 나중에 Damage로 바꾸기
		}
	}
	return ActualDamage;
}

void AMinion::DoRagdoll()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void AMinion::FlashRed()
{
	if (GetMesh())
	{
		GetMesh()->SetVectorParameterValueOnMaterials(TEXT("OverlayColor"), FVector(10.0f, 0.0f, 0.0f));
		
		GetWorldTimerManager().SetTimer(FlashTimer, [this]()
		{
			if (GetMesh())
			{
				GetMesh()->SetVectorParameterValueOnMaterials(TEXT("OverlayColor"), FVector(0.0f, 0.0f, 0.0f));
			}
		}, 0.1f, false);
	}
}


void AMinion::PlayMontage(UAnimMontage* MontageToPlay)
{
	PlayAnimMontage(MontageToPlay);
}




