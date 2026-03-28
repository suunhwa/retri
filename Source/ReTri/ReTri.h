// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(jiwon, Log, All);

#define LL *CALLINFO
#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define SCREENLOG(fmt, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("%s %s"), *CALLINFO, *FString::Printf(TEXT(fmt), ##__VA_ARGS__)))

#define JIWONLOG(fmt, ...) UE_LOG(jiwon, Warning, TEXT("%s %s"), *CALLINFO, *FString::Printf(TEXT(fmt), ##__VA_ARGS__))
