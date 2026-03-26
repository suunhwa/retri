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
	
	// 분신검기 사용했는지
	UPROPERTY(BlueprintReadWrite, Category = BossStatus)
	bool bHasPlayedMirrorBlade = false;
	
public:
	UPROPERTY(BlueprintReadOnly, Category = Skill)
	TArray<FDataTableRowHandle> BossSkills;
	
	UPROPERTY(EditdefaultsOnly, Category = StateTree);
	class UStateTreeComponent* StateTreeComponent;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
	virtual void OnAttackOverlap(AActor* OtherActor);
	
	void SetCurrentSkillDamage(float NewDamage) {CurrentSkillDamage = NewDamage;}

};
