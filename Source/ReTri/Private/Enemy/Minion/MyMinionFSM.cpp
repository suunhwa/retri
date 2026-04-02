// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Minion/MyMinionFSM.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/Minion/Minion.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyData.h"
#include "Kismet/GameplayStatics.h"

class AAIController;
// Sets default values for this component's properties
UMyMinionFSM::UMyMinionFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
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


// Called every frame
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
	case EMinionState::Damage:
		DamageState();
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
	//DrawDebugSphere(GetWorld(), Me->GetActorLocation(), AttackRange, 20, FColor::Cyan);
	
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
	
	Me->OnAttackOverlap(PlayerTarget);
	
	float Distance = Me->GetDistanceTo(PlayerTarget);

	if (Distance > AttackRange)
	{
		_State = EMinionState::Move;
		AIC->MoveToActor(PlayerTarget, 150.0f);
		return;
	}
	
	if (MyStatInfo->MinionAttackMontage)
	{
		Me->PlayMontage(MyStatInfo->MinionAttackMontage);
	}
	
	// float Distance = FVector::Distance(Me->GetActorLocation(), PlayerTarget->GetActorLocation());
	// if (Distance > AttackRange)
	// {
	// 	_State = EMinionState::Move;
	// }
}

void UMyMinionFSM::DamageState()
{
	
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





