// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyData.h"
#include "Components/StateTreeComponent.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMinionDieDelegate);

class UCameraShakeBase;
class UNIagaraSystem;

UCLASS()
class RETRI_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AEnemyBase();
	
	FMinionDieDelegate OnMinionDieDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	class UAnimMontage* DieMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	class UAnimMontage* JumpMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	class UAnimMontage* DownMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	class UAnimMontage* CloneAttackMontage;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX)
	TSubclassOf<class UCameraShakeBase> DeathCameraShake;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX)
	class UNiagaraSystem* DeathVFX;
	
public:
	// 스킬 사용 중인지
	UPROPERTY(BlueprintReadOnly)
	bool bIsSkillExecuting = false;
	
	// (1스킬) 차징 상태인지
	bool bIsCharging = false;
	
	// (1스킬) 차징하면서 바라볼 상대
	UPROPERTY()
	AActor* TargetActor = nullptr;
	
	// (3스킬)
	bool bIsJumpDownAttacking = false;
	
	// 이미 누군가를 때렸는지
	bool bHasHitTarget = false;
	
	// (4스킬)
	bool bIsMirrorPatternActive = false;
	
	// 바라보면서 회전할지 여부
	UPROPERTY(BlueprintReadWrite, Category = "Boss")
	bool bCanRotate = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EnemySetup)
	bool bIsBoss = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsDead = false; // 보스 죽었는지 여부
	

	// ----------- JumpDown -----------
	// 에디터에서 원본 머티리얼이나 인스턴스를 할당받는 용도w
	UPROPERTY(EditAnywhere, Category = Skill)
	UMaterialInterface* JumpCircleDecal;
	
	// BaseMaterial을 복사해서 메모리에 올리고 실시간으로 값을 수정할 때 사용
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicJumpCircleDecal;
	
	UPROPERTY()
	UDecalComponent* CircleDecalComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float JumpDownDamageRadius = 700.0f; // 장판(데칼)의 크기와 비슷하게 맞춤
	
	
	// ----------- MirrorBlade -----------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	UMaterialInterface* BoxDecal;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicBoxDecal;
	
	//UPROPERTY()
	//UDecalComponent* BoxDecalComp;
	
	UPROPERTY()
	TArray<AEnemyBase*> ActiveClones; // 분신 관리 배열
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Skill)
	TSubclassOf<AEnemyBase> CloneClass;
	
	UPROPERTY()
	UDecalComponent* ActiveDecal = nullptr;
	
	
	
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
	void StartCharging(AActor* NewTarget);	// Task에서 호출할 차징 함수
	void StopCharging();					// 차징 스탑! (돌진 직전에 호출)
	void RotateToTarget(float DeltaTime, float InterpSpeed);	// 플레이어 바라보는 함수
	
	void SpawnJumpDecal(FVector Location, class UMaterialInterface* JumpCircleDecal);
	
	void SpawnClones();
	void InitCloneDecal(float Width, float Length);	// 장판
	void StartDecalProgress(float Duration);
	
	UFUNCTION(BlueprintCallable)
	void ExecuteReinforcedDash(FVector StartLoc, FRotator DashRot);
	
	void PlayDeathEffect();
	void BroadcastDeath();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteJumpDownDamage();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ExecuteMirrorBladeDamage(AEnemyBase* Clone);
	
protected:
	virtual void UpdatePhase() { }
	
};
