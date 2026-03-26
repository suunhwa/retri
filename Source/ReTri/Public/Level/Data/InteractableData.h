// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h" 
#include "InteractableData.generated.h"


// -------------------------------------------------------------------
// [ 상호작용 기물 ]
// -------------------------------------------------------------------

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	Sanctuary	UMETA(DisplayName = "Sanctuary"),   // 성소
	Well		UMETA(DisplayName = "Well"),        // 우물
	Chaos		UMETA(DisplayName = "Chaos"),       // 혼돈
	Curse		UMETA(DisplayName = "Curse"),       // 저주
	Remnants	UMETA(DisplayName = "Remnants"),     // 기억의 잔재
	Portal		UMETA(DisplayName = "Portal")     // 기억의 잔재
};

USTRUCT(BlueprintType)
struct FInteractableData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InteractName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractableType InteractableType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> InteractableMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AInteractableBase> InteractableClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};

// -------------------------------------------------------------------
// [ Map 정보 ]
// -------------------------------------------------------------------

UENUM(BlueprintType)
enum class EMapNodeType : uint8
{
	Start		UMETA(DisplayName = "Start"),	// 시작맵
	Combat		UMETA(DisplayName = "Combat"),  // 전투맵
	Merchant	UMETA(DisplayName = "Merchant"),// 상점맵
	Boss		UMETA(DisplayName = "Boss")		// 보스맵
};

USTRUCT(BlueprintType)
struct FMapUIData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MapName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MapIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor IconColor = FLinearColor::White;
};

// 맵에 들어갈 오브젝트 및 정보
USTRUCT(BlueprintType)
struct FMapNodeData
{
	GENERATED_BODY()
	
public:
	// 방 타입
	UPROPERTY(BlueprintReadWrite)
	EMapNodeType MapType = EMapNodeType::Start;
	
	// 상호작용 기물 배열
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> SpawnInteractableRowNames;
	
	// 현재 방 번호
	UPROPERTY(BlueprintReadWrite)
	int32 MapIndex = 0;
	
	// 클리어 여부
	UPROPERTY(BlueprintReadWrite)
	bool bIsCleared = false;
	
	// 층 수가 높아질수록 위로 올라감 (Y축 감소)
	// X좌표 : 1층에 3개의 방이 생겼다면 서로 겹치지 않게 화면을 3등분 해 가로축으로 띄워놓음
	UPROPERTY(BlueprintReadWrite)
	FVector2D UIPosition;
	
	// 양방향 거미줄 맵 : 현재 방과 연결된 다른 방들의 번호 표
	// 시작 방 (0)에서 1번, 2번 방이 연결되어 있다면 이 배열엔 {1, 2}가 들어가있음.
	// 반대로 1번방에는 이 방번호가 들어가 있어야함 그래야 돌아갈 수 있음
	UPROPERTY(BlueprintReadWrite)
	TArray<int32> ConnectMapIndexs;
};
