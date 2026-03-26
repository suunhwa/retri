// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyBase.h"
#include <Codecapi.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (StateTreeComponent)
	{
		StateTreeComponent->StartLogic();
	}
	
	// 데이터테이블 존재 && 이름 존재
	if (StatDataTable != nullptr && !EnemyRowName.IsNone())
	{
		// 엑셀에서 이름과 일치하는 줄 가져오기
		FEnemyDataTableRow* MyStatInfo = StatDataTable->FindRow<FEnemyDataTableRow>(EnemyRowName, TEXT("Enemy Stat Lookup"));
		
		// 데이터를 찾았다면 내 몸에 적용
		if (MyStatInfo != nullptr)
		{
			MaxHP = MyStatInfo->MaxHP;
			CurrentHP = MaxHP;
			
			if (GetCharacterMovement())
			{
				GetCharacterMovement()->MaxWalkSpeed = MyStatInfo->MoveSpeed;
			}
			
			// BasicAttack = MyStatInfo->BasicAttackID;
			BossSkills = MyStatInfo->BossSkillsID;
			
			UE_LOG(LogTemp, Warning, TEXT("성공!! %s의 체력은 %f, 스킬은 %d개"), *EnemyRowName.ToString(), CurrentHP, BossSkills.Num());
		}
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemyBase::OnAttackOverlap(AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		// 엔진 공용 함수: "이 액터에게 이만큼 데미지를 줘!"
		UGameplayStatics::ApplyDamage(
			OtherActor,          // 맞은 대상 (플레이어)
			CurrentSkillDamage,     // 데이터 테이블에서 가져온 데미지
			GetController(),     // 공격 주체의 컨트롤러
			this,                // 공격 원인 (보스 자신)
			UDamageType::StaticClass() // 데미지 유형
		);
	}
}

float AEnemyBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

