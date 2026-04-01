// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Templates/SubclassOf.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "InteractableData.generated.h"

class AInteractableBase;

// === [ Interaction Info ] ===

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	Sanctuary	UMETA(DisplayName = "Sanctuary"),   // 성소
	Well		UMETA(DisplayName = "Well"),        // 우물
	Chaos		UMETA(DisplayName = "Chaos"),       // 혼돈
	Curse		UMETA(DisplayName = "Curse"),       // 저주
	Remnants	UMETA(DisplayName = "Remnants"),    // 기억의 잔재
	Portal		UMETA(DisplayName = "Portal")		// 균열
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

UENUM(BlueprintType)
enum class EChaosType : uint8
{
	Chaos_Health		UMETA(DisplayName="Chaos_Health"),		// 꺾이지 않는 의지
	Chaos_AttackDamage	UMETA(DisplayName="Chaos_AttackDamage"),  // 거인의 근력
	Chaos_AbilityPower	UMETA(DisplayName="Chaos_AbilityPower"),	// 무한한 지식
	Chaos_AttackSpeed	UMETA(DisplayName="Chaos_AttackSpeed"),   // 맹렬한 분노
	Chaos_MemoryHaste	UMETA(DisplayName="Chaos_MemoryHaste")	// 기민한 정신
};

USTRUCT(BlueprintType)
struct FChaosData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ChaosName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ChaosInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EChaosType ChaosType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ChaosLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> ChaosValues;
};

UENUM(BlueprintType)
enum class ECurseType : uint8
{
	Curse_Light		UMETA(DisplayName = "Curse_Light"),   // 미약한 증오
	Curse_Strong	UMETA(DisplayName = "Curse_Strong"),  // 강력한 증오
	Curse_Extreme	UMETA(DisplayName = "Curse_Extreme")  // 극렬한 증오
};

USTRUCT(BlueprintType)
struct FCurseData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurseName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurseInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECurseType CurseType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor CurseColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurseMonster;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurseMap;
};

UENUM(BlueprintType)
enum class ERewardType: uint8
{
	RewardGold			UMETA(DisplayName="RewardGold"),			// 
	RewardDreamPowder	UMETA(DisplayName="RewardDreamPowder"),		// 
	RewardMaxHP			UMETA(DisplayName="RewardMaxHP"),			// 
	RewardAttackDamage	UMETA(DisplayName="RewardAttackDamage"),	// 
	RewardMemoryHaste	UMETA(DisplayName="RewardMemoryHaste")		// 
};

USTRUCT(BlueprintType)
struct FCurseRewardData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERewardType RewardType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> RewardLevels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Info;
};

// === [ Map Info ] ===

UENUM(BlueprintType)
enum class EMapNodeType : uint8
{
	Start		UMETA(DisplayName = "Start"),	 // 시작맵
	Combat		UMETA(DisplayName = "Combat"),   // 전투맵
	Merchant	UMETA(DisplayName = "Merchant"), // 상점맵
	Boss		UMETA(DisplayName = "Boss")		 // 보스맵
};

USTRUCT(BlueprintType)
struct FMapUIData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MapName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> MapIcon;
	
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
	TMap<FName, bool> SpawnInteractableRowNames;
	
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
