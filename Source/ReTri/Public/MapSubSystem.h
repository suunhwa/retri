// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReTri/ReTri.h"
#include "ReTriGameData.h"
#include "Level/Data/InteractableData.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "MapSubSystem.generated.h"

class AActor;
class ALootGoldCoinPot;
class ALootDreamPowderPillar;

UENUM(BlueprintType)
enum class EActiveCurseQuest : uint8
{
	None,
	KillMinions,
	ClearMaps
};

USTRUCT(BlueprintType)
struct FActiveCurseQuest
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Curse")
	EActiveCurseQuest QuestType = EActiveCurseQuest::None;
	
	UPROPERTY(BlueprintReadWrite, Category="Curse")
	int32 TargetCount = 0;
	
	UPROPERTY(BlueprintReadWrite, Category="Curse")
	int32 CurCount = 0;
	
	UPROPERTY(BlueprintReadWrite, Category="Curse")
	ERewardType RewardType;
	
	UPROPERTY(BlueprintReadWrite, Category="Curse")
	int32 RewardValue = 0;
	
	UPROPERTY(BlueprintReadWrite, Category="Curse")
	FString RewardInfo;
};


UCLASS()
class RETRI_API UMapSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// === Infrastructure (UE Overrides) ===
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
public:
	// === Configuration (Set by GameInstance) ===
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Map|Config")
	UDataTable* MapUIData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Map|Config")
	UDataTable* InteractionData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Map|Config")
	UDataTable* SkillDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|Config")
	TSubclassOf<ALootGoldCoinPot> GoldCoinPotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|Config")
	TSubclassOf<ALootDreamPowderPillar> DreamPowderClass;

	// === Runtime Data (Managed by Subsystem) ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|Runtime")
	TArray<FMapNodeData> CurMapDatas;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|Runtime")
	TMap<int32, FShopItemSkillData> MerchantItemDatas;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|Runtime")
	TArray<FActiveCurseQuest> ActiveCurseQuests;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|Runtime")
	int32 CurMapIndex = 0;
	
	/** 현재 맵 정보 */
	UFUNCTION(BlueprintCallable, Category="Map|Helper")
	FMapNodeData GetCurMapData() { return CurMapDatas[CurMapIndex]; }
	
	/** 현재 맵 정보 (원본 직접 참조) */
	FMapNodeData& GetCurMapDataRef() { return CurMapDatas[CurMapIndex]; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|Runtime")
	int32 MinionSpawnerCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|Runtime")
	int32 CurClearSpawnerCount = 0;
	
	// === Public API ===
	/** 트리 구조 맵 생성 — ProceduralGenerateMap 사용 권장 */
	void GenerateMap();
	
	/** 절차적 맵 생성 (그리드 형태) */
	UFUNCTION(BlueprintCallable, Category="Map|Generation")
	void ProceduralGenerateMap();
	
	/** 노드 선택 시 해당 맵으로 진입 */
	UFUNCTION(BlueprintCallable, Category="Map|Flow")
	void EnterMap(int32 MapIndex);
	
	// === Helpers / Utilities ===
	UFUNCTION(BlueprintCallable, Category="Map|Helper")
	TMap<FName, bool> RandomInteractable(int32 RandomNum);
	
	UFUNCTION(BlueprintCallable, Category="Map|Helper")
	FInteractableData GetRowInteractionData(FName RowName, bool& bSuccess); 
	
	TArray<FPlayerSkillData*> GetRandomAcquiredItemList();
	
	// === Level Setting API? ===
	/** 사용된 Interactable 저장 */
	UFUNCTION(BlueprintCallable, Category="Map|State")
	void SetInteractableUsed(FName InRowName);
	
	/** Interactable 설치하는 함수 */
	UFUNCTION(BlueprintCallable, Category="Map|LevelSetting")
	void SpawnInteractable(TArray<AActor*> TargetPoints);
	
	/** 포탈 설치하는 함수 */
	UFUNCTION(BlueprintCallable, Category="Map|LevelSetting")
	void SpawnPortal(AActor* TP);

	/** 금화, 꿈가루 설치하는 함수 */
	UFUNCTION(BlueprintCallable, Category="Map|LevelSetting")
	void SpawnLootPieces(TArray<AActor*> TargetPoints);
	
	/** 상점 맵 마다 상인 상점 목록 생성 함수 */
	UFUNCTION(BlueprintCallable, Category="Map|LevelSetting")
	void SetMerchantItemList(int32 MapIndex);
	void RemoveMerchantItemList(int32 CurrentMap,int32 RemoveItemSlotNum);
	
	/** 적 스포너 개수 받아오기 */
	UFUNCTION(BlueprintCallable, Category="Map|LevelSetting")
	void SetMinionSpawnerCount();
	
	/** 게임 클리어 (포탈 생성 조건) */
	UFUNCTION(BlueprintCallable, Category="Map|LevelSetting")
	void LevelClear();
	
	// === Curse Quest ===
	/** 저주 퀘스트 추가 */
	UFUNCTION(BlueprintCallable, Category="Map|Curse")
	void AddCurseQuest(FActiveCurseQuest NewQuest);

	/** 저주 퀘스트 갱신 (맵 클리어, 처치 등) */
	UFUNCTION(BlueprintCallable, Category="Map|Curse")
	void UpdateCurseQuest(EActiveCurseQuest Type, int32 Amount = 1);
};
