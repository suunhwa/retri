// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Minion/MyMinionFSM.h"

#include "AIController.h"
#include "Enemy/Minion/Minion.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyData.h"
#include "Kismet/GameplayStatics.h"

class AAIController;
UMyMinionFSM::UMyMinionFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMyMinionFSM::BeginPlay()
{	
	Super::BeginPlay();
	
	Me = Cast<AMinion>(GetOwner());
	PlayerTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (Me)
	{
		MyStatInfo = Me->GetStatInfo();
		AIC = Cast<AAIController>(Me->GetController());
	}
	
	AnimInst = Me->GetMesh()->GetAnimInstance();
}


void UMyMinionFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	switch (_State)
	{
	case EMinionState::Idle:
		IdleState();
		break;
	case EMinionState::Move:
		MoveState();
		break;
	case EMinionState::Attack:
		AttackState();
		break;
	case EMinionState::Die:
		DieState();
		break;
	}
}

void UMyMinionFSM::IdleState()
{
	CurrentTime +=GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > IdleTime)
	{
		_State = EMinionState::Move;
		CurrentTime = 0.0f;
	}
}

void UMyMinionFSM::MoveState()
{
	if (AIC)
	{
		AIC->MoveToActor(PlayerTarget, 150.0f);
	}
	
	FVector Dir = PlayerTarget->GetActorLocation() - Me->GetActorLocation();
	
	float Distance = Dir.Size();
	
	if (Distance <= AttackRange)
	{
		_State = EMinionState::Attack;
	}
}

void UMyMinionFSM::AttackState()
{
	if (!Me || !MyStatInfo || !AIC) return;
	
	
	if (AnimInst->Montage_IsPlaying(MyStatInfo->MinionAttackMontage))
	{
		AIC->StopMovement();
		return;
	}
	
	float Distance = Me->GetDistanceTo(PlayerTarget);
	if (Distance > AttackRange)
	{
		_State = EMinionState::Move;
		AIC->MoveToActor(PlayerTarget, 150.0f);
		return;
	}
	
	Me->OnAttackOverlap(PlayerTarget);
	
	if (MyStatInfo->MinionAttackMontage)
	{
		Me->PlayMontage(MyStatInfo->MinionAttackMontage);
	}
}


void UMyMinionFSM::DieState()
{
	if (bIsDead) return;
		
	bIsDead = true;
	AIC->StopMovement();
	
	Me->DoRagdoll();
	
	Me->BroadcastDeath();
	
	if (DieAnim)
	{
		Me->PlayMontage(DieAnim);
	}
	
	Me->SetLifeSpan(2.0f);
}





