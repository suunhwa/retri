// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyData.h"
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
	
protected:
	// 선택할 데이터 테이블
	UPROPERTY(EditDefaultsOnly, Category = EnemySetup)
	UDataTable* StatDataTable;
	
	// 이름 (노말or보스)
	UPROPERTY(EditDefaultsOnly, Category = EnemySetup)
	FName EnemyRowName;
	
	UPROPERTY(BlueprintReadOnly, Category = Status)
	float CurrentHP;
	
	UPROPERTY(BlueprintReadOnly, Category = Status)
	float MaxHP;
	
	UPROPERTY(BlueprintReadOnly, Category = Skill)
	FDataTableRowHandle BasicAttack;
	UPROPERTY(BlueprintReadOnly, Category = Skill)
	TArray<FDataTableRowHandle> BossSkills;

};
