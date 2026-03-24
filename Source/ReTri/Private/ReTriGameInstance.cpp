// Fill out your copyright notice in the Description page of Project Settings.


#include "ReTriGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UReTriGameInstance::Init()
{
	Super::Init();
	
	if (InteractionData)
		UE_LOG(LogTemp, Warning, TEXT("[UReTriGameInstance] InteractionData 로드완료/ 행 수:%d"), InteractionData->GetRowNames().Num())
	else
		UE_LOG(LogTemp, Warning, TEXT("[UReTriGameInstance] InteractionData 할당안됨"));
	
	GenerateMap();
	EnterMap(1);
}

void UReTriGameInstance::GenerateMap()
{
	// TODO: 1단계 낡은 맵 정보 배열(CurrentMapData)을 모두 비우고, 내 위치(CurrentNodeIndex)를 0번(시작 방)으로 초기화하세요.
	CurMapDatas.Empty();
	CurMapIndex = 0;
	
	// [TODO: 2단계] 엑셀 데이터(InteractionData)가 없으면 엔진이 터지니까, 언리얼 로그를 띄우고 함수를 멈추게 하세요 (return).
	if (!InteractionData)
	{
		UE_LOG(LogTemp, Error, TEXT("[UReTriGameInstance] No InteractionData"));
		return;
	}
	
	// ----------------------------------------------------
	// TODO: 3단계 시작 방(Start) 만들기 (맵의 무조건적인 기준점!)
	// ----------------------------------------------------
	// 새로운 방 데이터(FMapNodeData)를 하나 만들고, NodeType을 Start로 지정합니다.
	FMapNodeData StartNode;
	StartNode.MapType = EMapNodeType::Start;
	StartNode.MapIndex = CurMapIndex;
	// 시작 구역은 평화로우므로 bIsCleared를 true로 지정한 뒤, 맵 정보 배열(CurrentMapData)에 0번으로 넣습니다.
	StartNode.bIsCleared = true;
	CurMapDatas.Add(StartNode);

	// ----------------------------------------------------
	// TODO: 4단계 거미줄처럼 갈래길 위주로 뻗어나가는 '가지 치기' (Tree 확장) 알고리즘
	// ----------------------------------------------------
	// 1. 방의 최대 깊이 (ex.4)
	int32 MaxDepth = 3; //! 멤버 변수
	// 2. "지금 방에서 가지를 뻗어야 하는 부모 방들의 인덱스 목록" 배열(CurrentDepthNodes)을 만듭니다.
	TArray<int32> CurDepthNodes;
	// 3. 처음엔 0번(금방 만든 시작 방)을 목록에 넣고 출발합니다!
	CurDepthNodes.Add(0); //! Start Map Index
	
	// [For 루프 시작!] 1층부터 MaxDepth층까지 돕니다.
	for (int32 Depth = 1; Depth < MaxDepth; Depth++)
	{
		// 이번 층에 새로 만들어진 가지(새 방)들을 기록할 임시 배열(NextDepthNodes)을 만듭니다.
		TArray<int32> ChildDepthNodes;
		
		// 현재 가지 끝에 매달린 모든 부모 방들(CurrentDepthNodes)에 대해 각각 For 루프를 돕니다.
		for (int32 ParentIndex : CurDepthNodes)
		{
			// 방 1개당 1개 ~ 3개 사이의 랜덤한 갈래길 개수(NumBranches)를 뽑습니다. (마지막 층은 1~2개로 줄여도 좋습니다)
			int32 RandomNode = FMath::RandRange(1, 3);
			
			// 또 For 루프를 돌아 갈래길 개수만큼 새로운 방 구조체를 생성합니다.
			for (int32 i = 0; i < RandomNode; i++)
			{
				FMapNodeData NewNode;
				
				// 1. 방의 테마 랜덤 설정 (80% 확률로 전투방, 20% 상점방) 및 bIsCleared를 false로 설정합니다.
				int32 RandomMapType = FMath::RandRange(1, 10);
				NewNode.MapType = (RandomMapType <= 2) ? EMapNodeType::Merchant : EMapNodeType::Combat;
				NewNode.bIsCleared = false;
				
				//todo 여기서 상호작용 랜덤으로 놓는거 그거 추가하고 
				//todo 금화, 꿈가루 설치도 여기서? 해야하나?
				
				// 2. 새 방을 CurrentMapData 배열에 새로 집어넣고(Add), 그 방금 들어간 고유 인덱스(NewNodeIndex)를 받아옵니다.
				int32 NewNodeIndex = CurMapDatas.Add(NewNode);
				CurMapDatas[NewNodeIndex].MapIndex = NewNodeIndex;
				
				const UEnum* EnumPtr = StaticEnum<EMapNodeType>();
				FString DisplayStr = EnumPtr->GetDisplayNameTextByValue((int64)NewNode.MapType).ToString();
				UE_LOG(LogTemp, Warning, TEXT("%d번째 맵 : %s"), NewNodeIndex, *DisplayStr);
				
				// 3. 다음 층 루프를 돌 때 이 방에서부터 다시 뻗어나갈 수 있도록 임시 배열(NextDepthNodes)에 NewNodeIndex를 저장합니다.
				ChildDepthNodes.Add(NewNodeIndex);
				
				// ----------------------------------------------------
				// [가장 중요한 핵심 - 양방향 다리 연결!] 
				// ----------------------------------------------------
				// 부모 방에서 이 신규 방으로 넘어올 수 있게 부모 방의 ConnectedNodeIndexes 배열에 NewNodeIndex를 Add합니다.
				CurMapDatas[ParentIndex].ConnectMapIndexs.Add(NewNodeIndex);
				// (갔던 곳을 갈 수 있게) 새 방에서도 부모 방으로 돌아갈 수 있게 새 방의 ConnectedNodeIndexes에 ParentNodeIndex를 Add해줍니다.
				CurMapDatas[NewNodeIndex].ConnectMapIndexs.Add(ParentIndex);
			}
		}
		// 이번 층 건설 완료! 다음 층 가지를 뻗칠 대상(CurrentDepthNodes)을 방금 모은 임시 배열(NextDepthNodes)로 덮어씌웁니다.
		CurDepthNodes = ChildDepthNodes;
		
		// ----------------------------------------------------
		// [TODO: 선택 사항 - '진짜 거미줄' 만들기! (방사형 지름길 뚫기)]
		// ----------------------------------------------------
		// 지금까지는 (부모 -> 자식)으로만 이어지는 '트리(Tree) 구조'입니다.
		// 만약 같은 층에 있는 남남인 방(다른 부모를 둔 자식)끼리도 서로 연결된 찐 거미줄을 원한다면?
		// 엄청 쉽습니다! 방금 갱신한 CurrentDepthNodes 배열 안에는 이번 층에 지어진 모든 방 번호들이 담겨 있습니다.
		// 그 배열 안에서 무작위로 방 A와 방 B의 번호를 뽑은 다음, 
		// 서로의 ConnectedNodeIndexes에 Add를 한 번씩 더해주면 방들이 횡방향으로 연결된 순환 그래프(거미줄)가 완성됩니다!
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
	// ----------------------------------------------------
	// TODO: 5단계 보스 방(Boss) 만들기
	// ----------------------------------------------------
	// 모든 갈래길 가지치기 루프가 끝났습니다.
	// CurrentDepthNodes 안에는 제일 끝자락(가장 먼)에 있는 잎사귀 방 번호들이 담겨 있습니다.
	// 이 중 랜덤한 방 인덱스를 하나 골라 해당 방의 NodeType을 Boss로 덮어씌웁니다.
	if (CurDepthNodes.Num() > 0)
	{
		int32 RandomBossIndex = CurDepthNodes[FMath::RandRange(0, CurDepthNodes.Num()-1)];
		CurMapDatas[RandomBossIndex].MapType = EMapNodeType::Boss;

		UE_LOG(LogTemp, Warning, TEXT("%d번째 맵 : Boss"), RandomBossIndex);
	}
	// [TODO: 완료 로그] 완성되었다면 CurrentMapData.Num() 개수를 언리얼 로그로 띄워주시면 생성 끝!
	UE_LOG(LogTemp, Warning, TEXT("맵 개수 : %d"), CurMapDatas.Num());
}

void UReTriGameInstance::EnterMap(int32 MapIndex)
{
	// TODO: 1단계 인자로 들어온 NodeIndex가 맵 배열 크기를 벗어나는 잘못된 번호가 아닌지 검사(IsValidIndex) 하세요.
	if (!CurMapDatas.IsValidIndex(MapIndex)) return;
	
	// TODO: 2단계 갈래길 양방향 이동 검증!
	// 현재 내가 있는 방의 정보를 배열에서 가져옵니다. 
	// 지금 방에서 사용자가 누른 목표 방(NodeIndex)과 "연결선(다리)"이 있는지 검사합니다.
	// 현재 방의 ConnectedNodeIndexes 배열 안에 목표 방 번호가 Contains()로 포함되어 있는지 체크하고, 없다면 return으로 이동을 막으세요!
	if (!CurMapDatas[CurMapIndex].ConnectMapIndexs.Contains(MapIndex)) return;
	
	// TODO: 3단계 모든 검증 박스를 패스했다면 나의 현재 빙의 위치(CurrentNodeIndex)를 목표 방의 번호(NodeIndex)로 갱신해 줍니다.
	CurMapIndex = MapIndex;

	// TODO: 4단계 이번 방의 테마(Start, Combat, Merchant, Boss)를 Switch-case문 등으로 확인하고, 각각에 어울리는 진짜 레벨(Level) 파일 이름(FName)을 결정하세요.
	switch (CurMapDatas[CurMapIndex].MapType)
	{
		case EMapNodeType::Start: 
		UE_LOG(LogTemp, Warning, TEXT("Start Map"));
		break;
		case EMapNodeType::Combat:
		UE_LOG(LogTemp, Warning, TEXT("Combat Map"));
		break;
		case EMapNodeType::Merchant:
		UE_LOG(LogTemp, Warning, TEXT("Merchant Map"));
		break;	
		case EMapNodeType::Boss:
		UE_LOG(LogTemp, Warning, TEXT("Boss Map"));
		break;
	}
	
	// TODO: 5단계 UGameplayStatics::OpenLevel 노드를 호출해 해당 맵 이름으로 화면을 텔레포트시키세요! 
	UE_LOG(LogTemp, Warning, TEXT("레벨 로드!!"));
}
