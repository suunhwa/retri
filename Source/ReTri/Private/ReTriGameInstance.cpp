// Fill out your copyright notice in the Description page of Project Settings.


#include "ReTriGameInstance.h"

void UReTriGameInstance::Init()
{
	Super::Init();
	
	if (InteractionData)
		UE_LOG(LogTemp, Warning, TEXT("[UReTriGameInstance] InteractionData 로드완료/ 행 수:%d"), InteractionData->GetRowNames().Num())
	else
		UE_LOG(LogTemp, Warning, TEXT("[UReTriGameInstance] InteractionData 할당안됨"));
		
}
