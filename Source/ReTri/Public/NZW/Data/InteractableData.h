// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InteractableData.generated.h"

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	None		UMETA(DisplayName = "None"),
	Sanctuary	UMETA(DisplayName = "Sanctuary"),   // 성소
	Well		UMETA(DisplayName = "Well"),        // 우물
	Chaos		UMETA(DisplayName = "Chaos"),       // 혼돈
	Curse		UMETA(DisplayName = "Curse"),       // 저주
	Remnants	UMETA(DisplayName = "Remnants")     // 기억의 잔재
};


USTRUCT(BlueprintType)
struct FInteractableData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	UPROPERTY(BlueprintReadWrite)
	FString KName;
	UPROPERTY(BlueprintReadWrite)
	EInteractableType InteractableType;
	UPROPERTY(BlueprintReadWrite)
	int32 Probability;
	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> StaticMesh;
	UPROPERTY(BlueprintReadWrite)
	FString Description;
};
