// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"


AEnemyAIController::AEnemyAIController()
{
	MyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("MyPerceptionComponent"));
	MyStateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("MyStateTreeComponent"));
	
	
}

