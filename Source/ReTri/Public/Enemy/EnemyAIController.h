// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/StateTreeComponent.h"
#include "EnemyAIController.generated.h"


UCLASS()
class RETRI_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();
	
	
protected:
	
	// 인지 시스템
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* MyPerceptionComponent;
	
	// StateTree 실행기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UStateTreeComponent* MyStateTreeComponent;
};
