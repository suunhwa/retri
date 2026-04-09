// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDropItem.generated.h"

/**
 * 보스 사망 시 드랍되는 아이템 마커 액터
 * 픽업 로직은 PlayerCharacter::OnPickUp에서 처리
 */
UCLASS()
class RETRI_API ABossDropItem : public AActor
{
	GENERATED_BODY()

public:
	ABossDropItem();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

public:
	// 전환할 레벨 이름 (비어있으면 로그만 출력)
	UPROPERTY(EditDefaultsOnly, Category="LevelTransition")
	FName NextLevelName = NAME_None;
};
