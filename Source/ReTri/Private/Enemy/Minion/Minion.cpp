// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Minion/Minion.h"

#include "Enemy/EnemyData.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"


AMinion::AMinion()
{
}

void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerTarget = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
}

void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerTarget)
	{
		CheckDistanceAndMove(DeltaTime);
	}
}

void AMinion::CheckDistanceAndMove(float DeltaTime)
{
	float Distance = GetDistanceTo(PlayerTarget);
	
    
	if (Distance > 300.0f) // 추격 범위
	{
		// 플레이어 향해 회전
		RotateToTarget(DeltaTime, 5.0f);

		// 이동
		if (AAIController* AIC = Cast<AAIController>(GetController()))
		{
			AIC->MoveToActor(PlayerTarget, 120.0f);
		}
	}
	else // 공격 범위
	{
		if (!bIsAttacking && MyStatInfo && MyStatInfo->MinionAttackMontage)
		{
			bIsAttacking = true;
			PlayAnimMontage(MyStatInfo->MinionAttackMontage);
            
			// 몽타주 종료 후 bIsAttacking을 false로 돌리는 로직 필요 (애님노티파이)
		}
	}
}

