// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReTriGameData.h"
#include "Engine/GameInstance.h"
#include "Level/Data/InteractableData.h"
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
	
	UFUNCTION()
	UDataTable* GetInteractionData() const { return InteractionData; }
	
//! 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Interaction")
	UDataTable* InteractionData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player|Stat")
	FPlayerStats SavePlayerStats;
	
	// -------------------------------------------------------------------
	// [절차적 맵 생성 데이터]
	// -------------------------------------------------------------------
	
	// 만들어진 모든 방(트리구조)들의 정보와 연결정보를 저장하는 배열
	// UI위젯에서 지도를 그릴 때 필요
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|NodeMap")
	TArray<FMapNodeData> CurMapDatas;
	
	// 현재 방번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Map|NodeMap")
	int32 CurMapIndex = 0;
	
//! 함수
	// 시작 방을 기준으로 여러 갈래로 뻗어나가며 끝 부분에 보스 방을 배치하는 다갈래길 맵 생성 함수
	UFUNCTION(BlueprintCallable, Category="Map|NodeMap")
	void GenerateMap();
	
	UFUNCTION(BlueprintCallable, Category="Map|NodeMap")
	void EnterMap(int32 MapIndex);

};
