
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
	Portal		UMETA(DisplayName = "Portal"),		// 균열
	Merchant	UMETA(DisplayName = "Merchant")		// 균열
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
	TSubclassOf<AInteractableBase> InteractableClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};


// === [ Chaos Data ] ===
UENUM(BlueprintType)
enum class EChaosType : uint8
{
	Chaos_Health		UMETA(DisplayName="Chaos_Health"),			// 꺾이지 않는 의지
	Chaos_AttackDamage	UMETA(DisplayName="Chaos_AttackDamage"),	// 거인의 근력
	Chaos_AbilityPower	UMETA(DisplayName="Chaos_AbilityPower"),	// 무한한 지식
	Chaos_AttackSpeed	UMETA(DisplayName="Chaos_AttackSpeed"),		// 맹렬한 분노
	Chaos_MemoryHaste	UMETA(DisplayName="Chaos_MemoryHaste")		// 기민한 정신
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


// === [ Curse Data ] ===
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
	RewardGold			UMETA(DisplayName="RewardGold"),			// 골드 보상
	RewardDreamPowder	UMETA(DisplayName="RewardDreamPowder"),		// 꿈가루 보상
	RewardMaxHP			UMETA(DisplayName="RewardMaxHP"),			// 최대 체력 보상
	RewardAttackDamage	UMETA(DisplayName="RewardAttackDamage"),	// 공격력 보상
	RewardMemoryHaste	UMETA(DisplayName="RewardMemoryHaste")		// 기억 가속 보상
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


// === [ Well Data ] ===
UENUM(BlueprintType)
enum class EWellRewardType: uint8
{
	RewardMaxHP					UMETA(DisplayName="RewardMaxHP"),				// 최대 체력 보상
	RewardAttackPower			UMETA(DisplayName="RewardAttackPower"),			// 공격력 보상
	RewardSpellPower			UMETA(DisplayName="RewardSpellPower"),			// 주문력 보상
	RewardProjectileSpeed		UMETA(DisplayName="RewardProjectileSpeed"),		// 총알 스피드 보상
	RewardAttackSpeed			UMETA(DisplayName="RewardAttackSpeed"),			// 공격 속도 보상
	RewardMoveSpeed				UMETA(DisplayName="RewardMoveSpeed"),			// 이동 속도 보상
	RewardMemoryAcceleration	UMETA(DisplayName="RewardMoveSpeed"),			// 기억 가속 보상
};

USTRUCT(BlueprintType)
struct FWellRewardData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WellRewardName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WellRewardNameKR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWellRewardType WellRewardType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DreamDustCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RewardAmount;
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

USTRUCT(BlueprintType)
struct FMapNodeData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	EMapNodeType MapType = EMapNodeType::Start;
	
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, bool> SpawnInteractableRowNames;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MapIndex = 0;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsCleared = false;
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D UIPosition;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<int32> ConnectMapIndexs;
};
