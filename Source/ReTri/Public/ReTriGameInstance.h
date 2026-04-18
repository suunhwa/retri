
#pragma once

#include "CoreMinimal.h"
#include "ReTriGameData.h"
#include "Engine/GameInstance.h"

#include "Player/Data/PlayerStatData.h"
#include "ReTriGameInstance.generated.h"

class UStatComponent;
class UHealthComponent;

UCLASS()
class RETRI_API UReTriGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	// === Player ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	FPlayerStatInfo CurPlayerStats;
	
	// 플레이어 컴포넌트 레퍼런스
	UPROPERTY()
	TObjectPtr<UStatComponent> StatComp;

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComp;
	
	// 스킬 데이터 (DT_PlayerSkills)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Data")
	UDataTable* SkillDataTable;

	// 레벨/경험치 데이터 (DT_LevelExp)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Data")
	UDataTable* LevelDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	bool bHasSavedStats = false;
	
	// 스킬 슬롯 매핑 저장 (드래그 앤 드롭으로 위치 바뀌어도 유지)
	// Key: EAbilitySlot, Value: 해당 슬롯에 장착된 스킬 클래스
	UPROPERTY()
	TMap<uint8, UClass*> SavedSkillSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	TSubclassOf<class UReTriGameData> GameDataClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Player|Stat")
	UReTriGameData* GameData;
	
	// 사망 시점 스탯 스냅샷 (레벨 전환 후에도 유지)
	UPROPERTY(BlueprintReadOnly, Category="Player|Stat")
	float Snapshot_MaxHP = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Player|Stat")
	float Snapshot_AttackPower = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Player|Stat")
	float Snapshot_SpellPower = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Player|Stat")
	float Snapshot_AttackSpeed = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Player|Stat")
	float Snapshot_MemoryAcceleration = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Player|Stat")
	float Snapshot_CritRate = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Player|Stat")
	float Snapshot_BurnDamageBonus = 0.f;
	UPROPERTY(BlueprintReadOnly, Category="Player|Stat")
	int32 Snapshot_CurrentExp = 0;

	/** 플레이어 사망 시 SaveStatSnapshot() 호출 → 컴포넌트 값을 GI에 복사 */
	void SaveStatSnapshot();
	
	void DebugStat() const;
	
	// === Level ===
	/** 상호작용 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Interaction")
	UDataTable* InteractionData;

	/** 절차적 맵 생성 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|NodeMap")
	UDataTable* MapUIData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|NodeMap")
	TSubclassOf<class ALootGoldCoinPot> GoldCoinPotClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|NodeMap")
	TSubclassOf<class ALootDreamPowderPillar> DreamPowderClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|NodeMap")
	TSubclassOf<class AFloatingUIActor> FloatingUIActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|GameOver")
	FPlayerPlayData PlayerPlayData;
	
};
