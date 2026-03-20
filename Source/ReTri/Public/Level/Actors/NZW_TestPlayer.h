// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NZW_TestPlayer.generated.h"

UCLASS()
class RETRI_API ANZW_TestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANZW_TestPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// // 기본 스탯
	// int32 Gold;
	// int32 DreamPowder;
	// float HP;
	// float AttackDamage;
	// float AbilityPower;
	// float Armor;
	//
	// // 보조 스탯
	// float AttackSpeed;
	// float CriticalChance;
	// float CriticalPower;
	// float MemoryHaste;
	//
	// // 유틸리티 특수 스탯
	// float MoveSpeed;
	// float ElemPower; // 화상 피해, 냉기 피해, 빛 피해 -> 원소스탯
	// float FireDamage;
	// float FrozenDamage;
	// float exp;
	
	// 스킬 레벨업 -> 
	
	//! 플레이어한테 이식할 < 함수 >
	// 마우스로 Interaction Object Hover했을 때 띄울 UI
	UFUNCTION(BlueprintCallable)
	void HoverInteractable();
	
	// F를 눌렀을 때 실행 할 함수
	UFUNCTION(BlueprintCallable)
	void Interaction();
	
	//! 플레이어한테 이식할 < 변수 >
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UReTriGameData* GD;
};
