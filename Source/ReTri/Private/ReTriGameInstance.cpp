// Fill out your copyright notice in the Description page of Project Settings.


#include "ReTriGameInstance.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Level/Actors/LootDreamPowderPillar.h"
#include "Level/Actors/LootGoldCoinPot.h"
#include "Runtime/Core/Tests/Containers/TestUtils.h"

void UReTriGameInstance::Init()
{
	Super::Init();
	
	if (InteractionData)
		UE_LOG(LogTemp, Warning, TEXT("[UReTriGameInstance] InteractionData 로드완료/ 행 수:%d"), InteractionData->GetRowNames().Num())
	else
		UE_LOG(LogTemp, Warning, TEXT("[UReTriGameInstance] InteractionData 할당안됨"));
	
	//GenerateMap();
	//EnterMap(1);
	ProceduralGenerateMap();
}

FInteractableData UReTriGameInstance::GetRowInteractionData(FName RowName, bool& bSuccess)
{
	bSuccess = false;
	
	// 1. 데이터 테이블 에셋 자체가 없는 경우 방어
	if (!InteractionData)
	{
		return FInteractableData(); 
	}
	FInteractableData* Row = InteractionData->FindRow<FInteractableData>(RowName, TEXT(""));
	
	// 2. 검색에 성공하여 유효한 데이터를 찾은 경우에만 역참조(*)하여 반환
	if (Row != nullptr) 
	{
		bSuccess = true;
		return *Row;
	}
	
	// 3. 데이터를 못 찾은 경우(nullptr), 엔진이 터지지 않게 빈 구조체를 만들어서 안전하게 반환
	return FInteractableData();
}

void UReTriGameInstance::SpawnInteractable(FName RowName, FVector Pos, FRotator Rot)
{
	bool bSuccess = false;
	FInteractableData Row = GetRowInteractionData(RowName, bSuccess);
	
}

void UReTriGameInstance::GenerateMap()
{
	// 1단계 낡은 맵 정보 배열 모두 비우고, 내 위치를 0번(시작 방)으로 초기화.
	CurMapDatas.Empty();
	CurMapIndex = 0;
	
	// 2단계 InteractionData 확인 예외처리
	if (!InteractionData)
	{
		UE_LOG(LogTemp, Error, TEXT("[UReTriGameInstance] No InteractionData"));
		return;
	}
	
	// 3단계 시작 방(Start) 만들기 (맵의 무조건적인 기준점!)
	// 새로운 방 데이터(FMapNodeData)를 하나 만들고, NodeType을 Start로 지정합니다.
	FMapNodeData StartNode;
	StartNode.MapType = EMapNodeType::Start;
	StartNode.MapIndex = CurMapIndex;
	StartNode.ConnectMapIndexs.Add(0);
	// 시작 구역은 평화로우므로 bIsCleared를 true로 지정한 뒤, 맵 정보 배열(CurrentMapData)에 0번으로 넣습니다.
	StartNode.bIsCleared = true;
	CurMapDatas.Add(StartNode);

	// 4단계 거미줄처럼 갈래길 위주로 뻗어나가는 '가지 치기' (Tree 확장) 알고리즘
	// - 1. 방의 최대 깊이 (ex.4)
	int32 MaxDepth = 3; //! 멤버 변수
	//! int32 MaxWidth = 4; 
	// - 2. 지금 방에서 가지를 뻗어야 하는 부모 방들의 인덱스 목록
	TArray<int32> CurDepthNodes;
	// - 3. 처음엔 0번(금방 만든 시작 방)을 목록에 넣기
	CurDepthNodes.Add(0); 
	
	// 1층부터 MaxDepth 돌면서 적용
	for (int32 Depth = 1; Depth < MaxDepth; Depth++)
	{
		// 이번 층에 새로 만들어진 가지(새 방)들을 기록할 임시 배열.
		TArray<int32> ChildDepthNodes;
		
		// 현재 가지 끝에 매달린 모든 부모 방들에 대해 각각 돌.
		for (int32 ParentIndex : CurDepthNodes)
		{
			// 방 1개당 1개 ~ 3개 사이의 랜덤한 갈래길 개수(NumBranches)를 뽑습니다. (마지막 층은 1~2개로 줄여도 좋습니다)
			int32 RandomNode = FMath::RandRange(2, 3);
			
			// 또 For 루프를 돌아 갈래길 개수만큼 새로운 방 구조체를 생성합니다.
			for (int32 i = 0; i < RandomNode; i++)
			{
				FMapNodeData NewNode;
				
				// 1. 방의 테마 랜덤 설정 (80% 확률로 전투방, 20% 상점방) 및 bIsCleared를 false로 설정합니다.
				int32 RandomMapType = FMath::RandRange(1, 10);
				NewNode.MapType = (RandomMapType <= 2) ? EMapNodeType::Merchant : EMapNodeType::Combat;
				NewNode.bIsCleared = false;
				
				int32 RandomNum = 0; /*! FMath::RandRange(1, 3); // 원래는 랜덤*/
				// 상호작용 기물 랜덤 배치를 위한 설정
				switch (NewNode.MapType)
				{
				case EMapNodeType::Combat:
					RandomNum = 3;
					break;
				case EMapNodeType::Merchant:
					RandomNum = 2;
					break;
				default: break;
				}
				NewNode.SpawnInteractableRowNames = RandomInteractable(RandomNum);
				
				// 2. 새 방을 CurrentMapData 배열에 새로 집어넣고(Add), 그 방금 들어간 고유 인덱스(NewNodeIndex)를 받아옵니다.
				int32 NewNodeIndex = CurMapDatas.Add(NewNode);
				CurMapDatas[NewNodeIndex].MapIndex = NewNodeIndex;
				
				const UEnum* EnumPtr = StaticEnum<EMapNodeType>();
				FString DisplayStr = EnumPtr->GetDisplayNameTextByValue((int64)NewNode.MapType).ToString();
				UE_LOG(LogTemp, Warning, TEXT("%d번째 맵 : %s"), NewNodeIndex, *DisplayStr);
				
				// 3. 다음 층 루프를 돌 때 이 방에서부터 다시 뻗어나갈 수 있도록 임시 배열(NextDepthNodes)에 NewNodeIndex를 저장합니다.
				ChildDepthNodes.Add(NewNodeIndex);
				
				// [가장 중요한 핵심 - 양방향 다리 연결!] 
				// 부모 방에서 이 신규 방으로 넘어올 수 있게 부모 방의 ConnectedNodeIndexes 배열에 NewNodeIndex를 Add합니다.
				CurMapDatas[ParentIndex].ConnectMapIndexs.Add(NewNodeIndex);
				// (갔던 곳을 갈 수 있게) 새 방에서도 부모 방으로 돌아갈 수 있게 새 방의 ConnectedNodeIndexes에 ParentNodeIndex를 Add해줍니다.
				CurMapDatas[NewNodeIndex].ConnectMapIndexs.Add(ParentIndex);
			}
		}
		// 이번 층 건설 완료! 다음 층 가지를 뻗칠 대상(CurrentDepthNodes)을 방금 모은 임시 배열(NextDepthNodes)로 덮어씌웁니다.
		CurDepthNodes = ChildDepthNodes;
		
		// '진짜 거미줄' 만들기! (방사형 지름길 뚫기)
		for (int32 i = 0; i < CurDepthNodes.Num()-1; i++)
		{
			int32 RandomMapType = FMath::RandRange(1, 10);
			if (RandomMapType >= 6)
			{
				int32 NodeA = CurDepthNodes[i];
				int32 NodeB = CurDepthNodes[i+1];
				CurMapDatas[NodeA].ConnectMapIndexs.Add(NodeB);
				CurMapDatas[NodeB].ConnectMapIndexs.Add(NodeA);
			}
		}
	} 
	
	// 5단계 보스 방(Boss) 만들기
	if (CurDepthNodes.Num() > 0)
	{
		int32 RandomBossIndex = CurDepthNodes[FMath::RandRange(0, CurDepthNodes.Num()-1)];
		CurMapDatas[RandomBossIndex].MapType = EMapNodeType::Boss;
		CurMapDatas[RandomBossIndex].SpawnInteractableRowNames.Empty();
		CurMapDatas[RandomBossIndex].SpawnInteractableRowNames.Add(TEXT("Sanctuary"));
		UE_LOG(LogTemp, Warning, TEXT("%d번째 맵 : Boss"), RandomBossIndex);
	}
	
	// 완료 로그 - 완성되었다면 CurrentMapData.Num() 개수를 언리얼 로그로 띄워주시면 생성 끝!
	UE_LOG(LogTemp, Warning, TEXT("맵 개수 : %d"), CurMapDatas.Num());
}

void UReTriGameInstance::ProceduralGenerateMap()
{
	// 1단계 낡은 맵 정보 배열 모두 비우고, 내 위치를 0번(시작 방)으로 초기화.
	CurMapDatas.Empty();
	CurMapIndex = 0;
	
	// 2단계 InteractionData 확인 예외처리
	if (!InteractionData)
	{
		UE_LOG(LogTemp, Error, TEXT("[UReTriGameInstance] No InteractionData"));
		return;
	}
	
	int32 MaxDepth = 4;
	int32 MinWidth = 1;
	int32 MaxWidth = 3;
	
	TArray<TArray<int32>> NodeGrid;
	NodeGrid.SetNum(MaxDepth);
	
	// 방생성 
	for (int32 Depth = 0; Depth < MaxDepth; Depth++)
	{
		// 레벨 Width 랜덤 정하기
		int32 Width = FMath::RandRange(MinWidth, MaxWidth);
		
		// UI를 위한 기본적 위치값 지정 -> 가운데에 배치하기 위해 계산 필요
		float XSpacing = 300.0f;
		float YSpacing = 150.0f;
		float StartX = (1920.0f - (XSpacing * MaxDepth + 400.0f)) / 2.0f;	// 가로 간격 MaxDepth * 100 + 400 (양 옆 200)
		float StartY = (1080.0f - (YSpacing * MaxWidth + 400.0f)) / 2.0f;	// 세로 간격 MaxWidth * 100 + 400 (양 위아래 200)
	
		// 현재 레벨 Width 만큼 돌면서
		for (int32 W = 0; W < Width; W++)
		{
			// 맵 생성
			FMapNodeData NewNode;
			
			// 맵테마 설정 (전투 80% / 상점 20%)
			int32 RandomMapType = FMath::RandRange(1, 10);
			NewNode.MapType = (RandomMapType <= 2) ? EMapNodeType::Merchant : EMapNodeType::Combat;
			NewNode.bIsCleared = false;
				
			// 상호작용 기물 스폰 랜덤 정하기
			int32 RandomNum = 0; /*! FMath::RandRange(1, 3); // 원래는 랜덤*/
			// 상호작용 기물 랜덤 배치를 위한 설정
			switch (NewNode.MapType)
			{
			case EMapNodeType::Combat:
				RandomNum = 3;
				break;
			case EMapNodeType::Merchant:
				RandomNum = 2;
				break;
			default: break;
			}
			NewNode.SpawnInteractableRowNames = RandomInteractable(RandomNum);
			
			// 위치 정해진 위치에서 +- 랜덤 위치 (지터링) RandomRange (-40, 40)
			float X = StartX + (Depth * XSpacing); 
			float Y = StartY + (W * YSpacing);
			float JitterX = FMath::RandRange(-40.f, 40.f);			
			float JitterY = FMath::RandRange(-40.f, 40.f);
			NewNode.UIPosition = FVector2D(X + JitterX, Y + JitterY);
			
			// 전역 배열에 맵추가 CurMapDatas
			int32 NewNodeIndex = CurMapDatas.Add(NewNode);
			CurMapDatas[NewNodeIndex].MapIndex = NewNodeIndex;
			// 깊이에도 저장 NodeGrid[Depth].Add
			NodeGrid[Depth].Add(NewNodeIndex);
	
			// 디버깅 출력
			const UEnum* EnumPtr = StaticEnum<EMapNodeType>();
			FString DisplayStr = EnumPtr->GetDisplayNameTextByValue((int64)CurMapDatas[NewNodeIndex].MapType).ToString();
			UE_LOG(LogTemp, Warning, TEXT("[ProceduralGenerateMap] %d번째 맵 (Depth:%d) : %s / Pos: %s"), NewNodeIndex, Depth, *DisplayStr, *NewNode.UIPosition.ToString());
		}
	}
	
	// 선 연결 -> 현재 열끼리 (옆 노드끼리), 다음 열의 노드와
	for (int32 Depth = 0; Depth < MaxDepth - 1; Depth++)
	{
		// 현재 열끼리 (옆 노드끼리)
		for (int32 i = 0; i < NodeGrid[Depth].Num()-1; i++)
		{
			int32 RandomMapType = FMath::RandRange(1, 10);
			if (RandomMapType >= 6)
			{
				int32 NodeA = NodeGrid[Depth][i];
				int32 NodeB = NodeGrid[Depth][i+1];
				CurMapDatas[NodeA].ConnectMapIndexs.Add(NodeB);
				CurMapDatas[NodeB].ConnectMapIndexs.Add(NodeA);
			}
		}
		
		// 다음 열의 노드와 연결
		TArray<int32>& CurDepth = NodeGrid[Depth];
		TArray<int32>& NextDepth = NodeGrid[Depth+1];
		
		for (int32 CurNodeIndex : CurDepth)
		{
			int32 EdgeNums = FMath::RandRange(1, NextDepth.Num());
			for (int32 i = 0; i < EdgeNums; i++)
			{
				int32 NextTargetIndex = NextDepth[FMath::RandRange(0, NextDepth.Num()-1)];
				if (!CurMapDatas[CurNodeIndex].ConnectMapIndexs.Contains(NextTargetIndex))
				{
					CurMapDatas[CurNodeIndex].ConnectMapIndexs.Add(NextTargetIndex);
					CurMapDatas[NextTargetIndex].ConnectMapIndexs.Add(CurNodeIndex);
				}
			}
		}
		
		// 길 잃은 노드 찾아서 연결
		for (int32 NextNodeIdx : NodeGrid[Depth+1]) 
		{
			bool bHasIncoming = false; // 들어오는 선이 있는가?
			// NextNodeIdx 방에 연결된 모든 선들 중에
			for(int32 cIdx : CurMapDatas[NextNodeIdx].ConnectMapIndexs) 
			{
				if (NodeGrid[Depth].Contains(cIdx)) 
				{ 
					bHasIncoming = true; // 이전 열에서 들어온 선 발견!
					break;
				}
			}
			// 들어온 선이 1개도 없다면 (길을 잃었다면)
			if (!bHasIncoming) 
			{
				// 1. 이전 층 방 목록(NodeGrid[Depth]) 중에서 무작위로 방 번호(인덱스) 1개 뽑기
				int32 RandomCurNodeIdx = NodeGrid[Depth][FMath::RandRange(0, NodeGrid[Depth].Num() - 1)];
    
				// 2. 길 잃은 내 방 <-> 뽑힌 이전 층 방, 양방향으로 억지로 선 연결해주기!
				CurMapDatas[RandomCurNodeIdx].ConnectMapIndexs.Add(NextNodeIdx);
				CurMapDatas[NextNodeIdx].ConnectMapIndexs.Add(RandomCurNodeIdx);
			}
		}
	}
	
	// 시작방 지정 -> Depth1~2라인에서 선택 (보류)
	if (NodeGrid[0].Num() > 0)
	{
		int32 StartIdxCandidate = NodeGrid[0][FMath::RandRange(0, NodeGrid[0].Num() - 1)];
		CurMapDatas[StartIdxCandidate].MapType = EMapNodeType::Start;
		CurMapDatas[StartIdxCandidate].bIsCleared = true; 
		CurMapDatas[StartIdxCandidate].SpawnInteractableRowNames.Empty();
		CurMapDatas[StartIdxCandidate].ConnectMapIndexs.Add(StartIdxCandidate);
		
		// 현재 캐릭터가 서 있는 빙의 위치를 시작 방 인덱스로 맞춰줍니다!
		CurMapIndex = StartIdxCandidate; 
	}
	
	// 보스방 지정 -> Depth 마지막 라인에서 선택 
	int32 LastDepth = MaxDepth - 1;
	if (NodeGrid[LastDepth].Num() > 0)
	{
		int32 BossIdxCandidate = NodeGrid[LastDepth][FMath::RandRange(0, NodeGrid[LastDepth].Num() - 1)];
		CurMapDatas[BossIdxCandidate].MapType = EMapNodeType::Boss;
		CurMapDatas[BossIdxCandidate].SpawnInteractableRowNames.Empty();
		CurMapDatas[BossIdxCandidate].SpawnInteractableRowNames.Add(TEXT("Sanctuary"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[GenerateMap_F] 맵 생성 완료! 총 노드 개수 : %d / 게임 셋업 시작 노드 인덱스: %d"), CurMapDatas.Num(), CurMapIndex);
}

TArray<FName> UReTriGameInstance::RandomInteractable(int32 RandomNum)
{
	TArray<FName> RowNames = InteractionData->GetRowNames();
	RowNames.Remove("Portal");
	
	TArray<FName> RandomNames;
	
	if (RowNames.Num() > 0)
	{
		while (RandomNum != 0)
		{
			int32 R = FMath::RandRange(0, RowNames.Num() - 1);
			if (RandomNames.Contains(RowNames[R])) continue;

			RandomNames.Add(RowNames[R]);
			RandomNum--;
		}
	}
	
	return RandomNames;
}

// 금화, 꿈가루 설치하는 함수
void UReTriGameInstance::SpawnLootPieces(ATargetPoint* TP)
{
	ALootBase* Loot = nullptr;
	
	if (TP->Tags.Contains("Gold") && GoldCoinPotClass)
	{
		Loot = GetWorld()->SpawnActor<ALootGoldCoinPot>(GoldCoinPotClass, TP->GetActorLocation(), TP->GetActorRotation());
	}
	else if (TP->Tags.Contains("Dream") && DreamPowderClass)
	{
		Loot = GetWorld()->SpawnActor<ALootDreamPowderPillar>(DreamPowderClass, TP->GetActorLocation(), TP->GetActorRotation());
	}
	
	Loot->SetActorLocation(Loot->GetActorLocation() + FVector(0.f, 0.f, Loot->CapsuleComp->GetScaledCapsuleHalfHeight()));
	UE_LOG(LogTemp, Warning, TEXT("아니ㅡㅡ 왜 뭐 왜 ㅁ눠!!!!!!!!"));
}

void UReTriGameInstance::EnterMap(int32 MapIndex)
{
	// 1단계 인자로 들어온 MapIndex가 맵 배열 크기를 벗어나는 잘못된 번호가 아닌지 검사
	if (!CurMapDatas.IsValidIndex(MapIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("맵 배열 크기를 벗어나는 잘못된 번호!!"));
		return;	
	}
	
	// 2단계 갈래길 양방향 이동 검증
	if (!CurMapDatas[CurMapIndex].ConnectMapIndexs.Contains(MapIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("멀리있는 장소 선택함! : %d"), MapIndex);
		return;	
	}
	
	// 3단계 모든 검증 박스를 패스했다면 나의 현재 빙의 위치를 목표 방의 번호로 갱신
	CurMapIndex = MapIndex;

	// 현재 방에 맞는 Level이름 적용
	FName LevelName;
	switch (CurMapDatas[CurMapIndex].MapType)
	{
		case EMapNodeType::Start: 
		LevelName = TEXT("Lv_StartMap");
		UE_LOG(LogTemp, Warning, TEXT("Start Map"));
		break;
		case EMapNodeType::Combat:
		LevelName = TEXT("Lv_BattleMap");
		UE_LOG(LogTemp, Warning, TEXT("Combat Map"));
		break;
		case EMapNodeType::Merchant:
		LevelName = TEXT("Lv_ShopMap");
		UE_LOG(LogTemp, Warning, TEXT("Merchant Map"));
		break;	
		case EMapNodeType::Boss:
		LevelName = TEXT("Lv_BossMap");
		UE_LOG(LogTemp, Warning, TEXT("Boss Map"));
		break;
	}
	
	// 5단계 UGameplayStatics::OpenLevel 노드를 호출해 해당 맵 이름으로 화면을 텔레포트
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	UE_LOG(LogTemp, Warning, TEXT("레벨 로드!!"));
}
