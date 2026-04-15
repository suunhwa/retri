// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy/EnemyData.h"
#include "Enemy/EnemyBase.h"
#include "MyMinionFSM.generated.h"

class AMinion;
class UAnimInstance;

UENUM(BlueprintType)
enum class EMinionState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Attack UMETA(DisplayName = "Attack"),
	Die UMETA(DisplayName = "Die")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RETRI_API UMyMinionFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMyMinionFSM();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	class ACharacter* PlayerTarget;
	
	FEnemyDataTableRow* MyStatInfo;
	
	class UAnimInstance* AnimInst;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	class UAnimMontage* DieAnim;
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSM)
	EMinionState _State = EMinionState::Idle;
	
	void IdleState();
	void MoveState();
	void AttackState();
	void DieState();
	
public:
	// Idle
	float CurrentTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = FSM)
	float IdleTime = 1.0f;
	
	// Move
	UPROPERTY()
	class AMinion* Me;
	UPROPERTY()
	class AAIController* AIC;
	UPROPERTY(EditAnywhere, Category = FSM)
	float AttackRange = 300.0f;

	// Die
	bool bIsDead = false;
};
