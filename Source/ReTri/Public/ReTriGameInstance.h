// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReTriGameData.h"
#include "Engine/GameInstance.h"
#include "Level/Data/InteractableData.h"
#include "Player/Data/PlayerStatData.h"
#include "ReTriGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RETRI_API UReTriGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	// =========== Level ===============================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	FPlayerStats NZWSavePlayerStats;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	FPlayerStatInfo CurPlayerStats;
	
	//! 상호작용 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Interaction")
	UDataTable* InteractionData;

	UFUNCTION(BlueprintCallable, Category="Player|Interaction")
	FInteractableData GetRowInteractionData(FName RowName, bool& bSuccess); 
	
	void SpawnInteractable(FName RowName, FVector Pos, FRotator Rot=FRotator::ZeroRotator);
	
	//! 절차적 맵 생성 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|NodeMap")
	UDataTable* MapUIData;
	
	// 만들어진 모든 방(트리구조)들의 정보와 연결정보를 저장하는 배열
	// UI위젯에서 지도를 그릴 때 필요
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|NodeMap")
	TArray<FMapNodeData> CurMapDatas;
	
	// 현재 방번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|NodeMap")
	int32 CurMapIndex = 0;
	
	UFUNCTION()
	UDataTable* GetMapUIData() const { return MapUIData; }
	
	// 시작 방을 기준으로 여러 갈래로 뻗어나가며 끝 부분에 보스 방을 배치하는 다갈래길 맵 생성 함수
	UFUNCTION(BlueprintCallable, Category="Map|NodeMap")
	void GenerateMap();
	
	// 시작 방을 기준으로 여러 갈래로 뻗어나가며 끝 부분에 보스 방을 배치하는 다갈래길 맵 생성 함수
	UFUNCTION(BlueprintCallable, Category="Map|NodeMap")
	void ProceduralGenerateMap();
	
	// 상호작용 기물 랜덤으로 선택해주는 함수 
	UFUNCTION(BlueprintCallable, Category="Map|NodeMap")
	TArray<FName> RandomInteractable(int32 RandomNum);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|NodeMap")
	TSubclassOf<class ALootGoldCoinPot> GoldCoinPotClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map|NodeMap")
	TSubclassOf<class ALootDreamPowderPillar> DreamPowderClass;
	
	// 골드항아리나 꿈가루기둥을 배치해주는 함수
	UFUNCTION(BlueprintCallable, Category="Map|NodeMap")
	void SpawnLootPieces(ATargetPoint* TP);
	
	// 노드 선택 시 맵 진입 함수
	UFUNCTION(BlueprintCallable, Category="Map|NodeMap")
	void EnterMap(int32 MapIndex);
	// =========== Level ===============================================
};
