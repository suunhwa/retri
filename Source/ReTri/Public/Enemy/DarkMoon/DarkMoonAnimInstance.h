// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "DarkMoonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API UDarkMoonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Anim)
	float Speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Anim)
	bool bIsInAir = false;
	
	
	
	
};
