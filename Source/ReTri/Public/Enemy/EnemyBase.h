// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyData.h"
#include "Components/StateTreeComponent.h"
#include "EnemyBase.generated.h"

UCLASS()
class RETRI_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	// 선택할 데이터 테이블
	UPROPERTY(EditDefaultsOnly, Category = EnemySetup)
	UDataTable* StatDataTable;
	
	FEnemyDataTableRow* MyStatInfo;
	
	// 이름 (노말or보스)
	UPROPERTY(EditDefaultsOnly, Category = EnemySetup)
	FName EnemyRowName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Status)
	float CurrentHP;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	float CurrentSkillDamage;
	
	UPROPERTY(BlueprintReadOnly, Category = Status)
	float MaxHP;
	UPROPERTY(BlueprintReadOnly, Category = Status)
	float MoveSpeed;
	
	UPROPERTY(BlueprintReadWrite, Category = BossStatus)
	int32 CurrentPhase = 1;
	
	// 분신검기(4스킬) 사용했는지
	UPROPERTY(BlueprintReadWrite, Category = BossStatus)
	bool bHasPlayedMirrorBlade = false;
	
	// 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	class UAnimMontage* ChargeMontage;
	
public:
	// (1스킬) 차징 상태인지
	bool bIsCharging = false;
	
	// (1스킬) 차징하면서 바라볼 상대
	UPROPERTY()
	AActor* TargetActor = nullptr;
	
	// 이미 누군가를 때렸는지
	bool bHasHitTarget = false;
	
	// 바라보면서 회전할지 여부
	UPROPERTY(BlueprintReadWrite, Category = "Boss")
	bool bCanRotate = true;

public:
	// 공격 시작 시 호출
	void ResetAttackHit() { bHasHitTarget = false; }

public:
	UPROPERTY(BlueprintReadOnly, Category = Skill)
	TArray<FDataTableRowHandle> BossSkills;
	
	UPROPERTY(EditdefaultsOnly, Category = StateTree);
	class UStateTreeComponent* StateTreeComponent;
	
	
	UFUNCTION()
	// virtual void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	virtual void OnAttackOverlap(AActor* OtherActor);	// Attack
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; // 대미지
	void SetCurrentSkillDamage(float NewDamage) {CurrentSkillDamage = NewDamage;} // 스킬 대미지 설정

public:
	void StartCharging(AActor* NewTarget);	// Task에서 호출할 함수
	void StopCharging();					// 차징 스탑! (돌진 직전에 호출)
	void RotateToTarget(float DeltaTime, float InterpSpeed);	// 플레이어 바라보는 함수
	
	
protected:
	virtual void UpdatePhase() { }
	
};
