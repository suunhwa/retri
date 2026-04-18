

#include "MapSubSystem.h"

#include "ReTriGameInstance.h"
#include "Level/Actors/LootDreamPowderPillar.h"
#include "Level/Actors/LootGoldCoinPot.h"
#include "Level/Actors/InteractableBase.h"
#include "Level/Actors/FloatingUIActor.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Components/HealthComponent.h"
#include "Player/Components/StatComponent.h"

// === Infrastructure (UE Overrides) ===
void UMapSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetGameInstance()))
	{
		InteractionData = GI->InteractionData;
		MapUIData = GI->MapUIData;
		SkillDataTable = GI->SkillDataTable;
		
		GoldCoinPotClass = GI->GoldCoinPotClass;
		DreamPowderClass = GI->DreamPowderClass;
		
		ProceduralGenerateMap();
	}
}

void UMapSubSystem::Deinitialize()
{
	Super::Deinitialize();
	
}

// === Public API ===
void UMapSubSystem::GenerateMap()
{
	CurMapDatas.Empty();
	CurMapIndex = 0;
	
	if (!InteractionData)
		return;
	
	FMapNodeData StartNode;
	StartNode.MapType = EMapNodeType::Start;
	StartNode.MapIndex = CurMapIndex;
	StartNode.ConnectMapIndexs.Add(0);
	StartNode.bIsCleared = false;
	CurMapDatas.Add(StartNode);

	int32 MaxDepth = 3; 
	TArray<int32> CurDepthNodes;
	CurDepthNodes.Add(0); 
	
	for (int32 Depth = 1; Depth < MaxDepth; Depth++)
	{
		TArray<int32> ChildDepthNodes;
		
		for (int32 ParentIndex : CurDepthNodes)
		{
			int32 RandomNode = FMath::RandRange(2, 3);
			
			for (int32 i = 0; i < RandomNode; i++)
			{
				FMapNodeData NewNode;
				
				int32 RandomMapType = FMath::RandRange(1, 10);
				NewNode.MapType = (RandomMapType <= 2) ? EMapNodeType::Merchant : EMapNodeType::Combat;
				NewNode.bIsCleared = false;
				
				int32 RandomNum = 0; 
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
				
				int32 NewNodeIndex = CurMapDatas.Add(NewNode);
				CurMapDatas[NewNodeIndex].MapIndex = NewNodeIndex;
				
				const UEnum* EnumPtr = StaticEnum<EMapNodeType>();
				FString DisplayStr = EnumPtr->GetDisplayNameTextByValue((int64)NewNode.MapType).ToString();
				
				ChildDepthNodes.Add(NewNodeIndex);
				
				CurMapDatas[ParentIndex].ConnectMapIndexs.Add(NewNodeIndex);
				CurMapDatas[NewNodeIndex].ConnectMapIndexs.Add(ParentIndex);
			}
		}
		
		CurDepthNodes = ChildDepthNodes;
		
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
	
	if (CurDepthNodes.Num() > 0)
	{
		int32 RandomBossIndex = CurDepthNodes[FMath::RandRange(0, CurDepthNodes.Num()-1)];
		CurMapDatas[RandomBossIndex].MapType = EMapNodeType::Boss;
		CurMapDatas[RandomBossIndex].SpawnInteractableRowNames.Empty();
		CurMapDatas[RandomBossIndex].SpawnInteractableRowNames.Add(TEXT("Sanctuary"));
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("맵 개수 : %d"), CurMapDatas.Num());
}

void UMapSubSystem::ProceduralGenerateMap()
{
	CurMapDatas.Empty();
	CurMapIndex = 0;
	
	MerchantItemDatas.Empty();
	ActiveCurseQuests.Empty();
	MinionSpawnerCount = 0;
	CurClearSpawnerCount = 0;
	
	if (!InteractionData) return;
	
	int32 MaxDepth = 4;
	int32 MinWidth = 2;
	int32 MaxWidth = 3;
	
	TArray<TArray<int32>> NodeGrid;
	NodeGrid.SetNum(MaxDepth);
	
	// 방 생성
	for (int32 Depth = 0; Depth < MaxDepth; Depth++)
	{
		int32 Width = FMath::RandRange(MinWidth, MaxWidth);
		
		float XSpacing = 300.0f;
		float YSpacing = 150.0f;
		float StartX = (1920.0f - XSpacing * MaxDepth) / 2.0f;	// 가로 간격 MaxDepth * 100 + 400 (양 옆 200)
		float StartY = (1080.0f - YSpacing * MaxWidth) / 2.0f;	// 세로 간격 MaxWidth * 100 + 400 (양 위아래 200)
		
		for (int32 W = 0; W < Width; W++)
		{
			FMapNodeData NewNode;
			
			int32 RandomMapType = FMath::RandRange(1, 10);
			NewNode.MapType = (RandomMapType <= 3) ? EMapNodeType::Merchant : EMapNodeType::Combat;
			NewNode.bIsCleared = false;
				
			int32 RandomNum = 0;
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
			
			float X = StartX + (Depth * XSpacing); 
			float Y = StartY + (W * YSpacing);
		
			// 위치 정해진 위치에서 +- 랜덤 위치 (지터링)
			float JitterX = FMath::RandRange(-40.f, 40.f);			
			float JitterY = FMath::RandRange(-40.f, 40.f);
			NewNode.UIPosition = FVector2D(X + JitterX, Y + JitterY);
			
			int32 NewNodeIndex = CurMapDatas.Add(NewNode);
			CurMapDatas[NewNodeIndex].MapIndex = NewNodeIndex;
			
			if (CurMapDatas[NewNodeIndex].MapType == EMapNodeType::Merchant)
			{
				SetMerchantItemList(NewNodeIndex);
			}
			
			NodeGrid[Depth].Add(NewNodeIndex);
	
			// const UEnum* EnumPtr = StaticEnum<EMapNodeType>();
			// FString DisplayStr = EnumPtr->GetDisplayNameTextByValue((int64)CurMapDatas[NewNodeIndex].MapType).ToString();
			// UE_LOG(LogTemp, Warning, TEXT("[ProceduralGenerateMap] %d번째 맵 (Depth:%d) : %s / Pos: %s"), NewNodeIndex, Depth, *DisplayStr, *NewNode.UIPosition.ToString());
		}
	}
	
	// 노드 연결
	for (int32 Depth = 0; Depth < MaxDepth - 1; Depth++)
	{
		// 현재 열끼리 연결
		for (int32 i = 0; i < NodeGrid[Depth].Num()-1; i++)
		{
			int32 RandomMapType = FMath::RandRange(0, 1);
			if (RandomMapType > 0)
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
		
		// 동 떨어진 노드 연결
		for (int32 NextNodeIdx : NodeGrid[Depth+1]) 
		{
			bool bHasIncoming = false; 
			
			for(int32 cIdx : CurMapDatas[NextNodeIdx].ConnectMapIndexs) 
			{
				if (NodeGrid[Depth].Contains(cIdx)) 
				{ 
					bHasIncoming = true;
					break;
				}
			}
			
			if (!bHasIncoming) 
			{
				int32 RandomCurNodeIdx = NodeGrid[Depth][FMath::RandRange(0, NodeGrid[Depth].Num() - 1)];
    
				CurMapDatas[RandomCurNodeIdx].ConnectMapIndexs.Add(NextNodeIdx);
				CurMapDatas[NextNodeIdx].ConnectMapIndexs.Add(RandomCurNodeIdx);
			}
		}
	}
	
	// 시작방 지정
	if (NodeGrid[0].Num() > 0)
	{
		int32 StartIdxCandidate = NodeGrid[0][FMath::RandRange(0, NodeGrid[0].Num() - 1)];
		CurMapDatas[StartIdxCandidate].MapType = EMapNodeType::Start;
		CurMapDatas[StartIdxCandidate].bIsCleared = false; 
		CurMapDatas[StartIdxCandidate].SpawnInteractableRowNames.Empty();
		CurMapDatas[StartIdxCandidate].ConnectMapIndexs.Add(StartIdxCandidate);
		
		CurMapIndex = StartIdxCandidate; 
	}
	
	// 보스방 지정
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

void UMapSubSystem::EnterMap(int32 MapIndex)
{
	if (!CurMapDatas.IsValidIndex(MapIndex)) return;	
	if (!CurMapDatas[CurMapIndex].ConnectMapIndexs.Contains(MapIndex)) return;	
	
	CurMapIndex = MapIndex;

	FName LevelName;
	switch (CurMapDatas[CurMapIndex].MapType)
	{
	case EMapNodeType::Start: 
		LevelName = TEXT("Lv_StartMap");
		break;
	case EMapNodeType::Combat:
		LevelName = TEXT("Lv_BattleMap");
		break;
	case EMapNodeType::Merchant:
		LevelName = TEXT("Lv_ShopMap");
		break;	
	case EMapNodeType::Boss:
		LevelName = TEXT("Lv_BossMap");
		break;
	}
	
	if (UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetGameInstance()))
	{
		GI->PlayerPlayData.SetVisitLevel(1);
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

// === Helpers / Utilities ===
TMap<FName, bool> UMapSubSystem::RandomInteractable(int32 RandomNum)
{
	TArray<FName> RowNames = InteractionData->GetRowNames();
	RowNames.Remove("Portal"); // 포탈은 제외
	
	TMap<FName, bool> RandomNames;
	
	int32 Count = FMath::Min(RandomNum, RowNames.Num());
	if (RowNames.Num() > 0)
	{
		while (Count != 0)
		{
			int32 R = FMath::RandRange(0, RowNames.Num() - 1);
			if (RandomNames.Contains(RowNames[R])) continue;
			
			RandomNames.Add(RowNames[R], false);
			Count--;
			
			FString RowName = RowNames[R].ToString();
		}
	}
	
	return RandomNames;
}

FInteractableData UMapSubSystem::GetRowInteractionData(FName RowName, bool& bSuccess)
{
	bSuccess = false;
	
	if (!InteractionData) return FInteractableData(); 
	FInteractableData* Row = InteractionData->FindRow<FInteractableData>(RowName, TEXT(""));
	
	if (Row != nullptr) 
	{
		bSuccess = true;
		return *Row;
	}
	
	return FInteractableData();
}

TArray<FPlayerSkillData*> UMapSubSystem::GetRandomAcquiredItemList()
{
	TArray<FPlayerSkillData*> AllSkills; 
	if (SkillDataTable)
	{
		SkillDataTable->GetAllRows<FPlayerSkillData>(TEXT("AllSkills"), AllSkills);
	}
    
	TArray<FPlayerSkillData*> AcquiredSkills; 
	for (auto Skill : AllSkills)
	{
		if (Skill->SkillCategory == ESkillCategory::Acquired)
		{
			if (Skill->SkillID.Equals(FString::Printf(TEXT("boss_drop"))))
			{
				BossDropItem = Skill;
				continue;
			}
			
			AcquiredSkills.Add(Skill);
		}
	}
	
	if (AcquiredSkills.Num() > 0)
	{
		int32 LastIndex = AcquiredSkills.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 RandomIndex = FMath::RandRange(i, LastIndex);
        
			if (i != RandomIndex)
			{
				AcquiredSkills.Swap(i, RandomIndex);
			}
		}
	}
	
	return AcquiredSkills;
}

// === Level Setting API? ===
void UMapSubSystem::SetInteractableUsed(FName InRowName)
{
	if (CurMapDatas.IsValidIndex(CurMapIndex))
	{
		if (CurMapDatas[CurMapIndex].SpawnInteractableRowNames.Contains(InRowName))
		{
			CurMapDatas[CurMapIndex].SpawnInteractableRowNames[InRowName] = true;
		}
	}
}

void UMapSubSystem::SpawnInteractable(TArray<AActor*> TargetPoints)
{
	TArray<AActor*> TPs;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Interactable"), TPs);
	
	TArray<FName> RowNames;
	CurMapDatas[CurMapIndex].SpawnInteractableRowNames.GetKeys(RowNames);
	for (int i = 0; i < RowNames.Num(); i++)
	{
		if (!TPs[i]) continue;
		
		bool bSuccess = false;
		FInteractableData IData = GetRowInteractionData(RowNames[i], bSuccess);
		
		if (!bSuccess) continue;
		
		AInteractableBase* I = GetWorld()->SpawnActor<AInteractableBase>(
			IData.InteractableClass, 
			TPs[i]->GetActorLocation(), 
			TPs[i]->GetActorRotation()
		);
		
		bool* bIsUsedPtr = CurMapDatas[CurMapIndex].SpawnInteractableRowNames.Find(RowNames[i]);
		bool bIsUsed = bIsUsedPtr ? *bIsUsedPtr : false;
		
		I->SetIsUsed(bIsUsed);
		I->DataInit(RowNames[i], IData);
	}
}

void UMapSubSystem::SpawnPortal(AActor* TP)
{
	bool bSuccess = false;
	FInteractableData IData = GetRowInteractionData(TEXT("Portal"), bSuccess);
		
	if (!bSuccess) return;
		
	AInteractableBase* I = GetWorld()->SpawnActor<AInteractableBase>(
		IData.InteractableClass, 
		TP->GetActorLocation(), 
		TP->GetActorRotation()
	);
		
	I->DataInit(TEXT("Portal"),IData);
}

void UMapSubSystem::SpawnLootPieces(TArray<AActor*> TargetPoints)
{
	ALootBase* Loot = nullptr;
	for (AActor* TP : TargetPoints)
	{
		if (TP->Tags.Contains("Gold") && GoldCoinPotClass)
		{
			Loot = GetWorld()->SpawnActor<ALootGoldCoinPot>(GoldCoinPotClass, TP->GetActorLocation(), TP->GetActorRotation());
		}
		else if (TP->Tags.Contains("Dream") && DreamPowderClass)
		{
			Loot = GetWorld()->SpawnActor<ALootDreamPowderPillar>(DreamPowderClass, TP->GetActorLocation(), TP->GetActorRotation());
		}
		
		if (Loot)
		{
			Loot->SetActorLocation(Loot->GetActorLocation() + FVector(0.f, 0.f, Loot->CapsuleComp->GetScaledCapsuleHalfHeight()));
		}
	}
}

void UMapSubSystem::SetMerchantItemList(int32 MapIndex)
{
	TArray<FPlayerSkillData*> AcquiredSkills = GetRandomAcquiredItemList();
	
	FShopItemSkillData TempSkillData;
	int32 PickCount = FMath::RandRange(4, 6); 
	if (PickCount > AcquiredSkills.Num()) PickCount = AcquiredSkills.Num();
	for (int32 i = 0; i < PickCount; ++i)
	{
		TempSkillData.ItemSkillDatas.Add(*AcquiredSkills[i]);
	}

	MerchantItemDatas.Add(MapIndex, TempSkillData);
}

void UMapSubSystem::RemoveMerchantItemList(int32 CurrentMap,int32 RemoveItemSlotNum)
{
	FShopItemSkillData* SkillDatas = MerchantItemDatas.Find(CurrentMap);
	
	if (!SkillDatas) return;
	
	if (!SkillDatas->ItemSkillDatas.IsValidIndex(RemoveItemSlotNum)) return;
	
	SkillDatas->ItemSkillDatas.RemoveAt(RemoveItemSlotNum);
	// SCREENLOG("현재 상점 아이템 개수: %d", SkillDatas->ItemSkillDatas.Num());
}

void UMapSubSystem::SetMinionSpawnerCount()
{
	TArray<AActor*> MinionSpawners;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MinionSpawner"), MinionSpawners);
	
	MinionSpawnerCount = MinionSpawners.Num();
	// SCREENLOG("미니언 스포너 개수: %d", MinionSpawnerCount);
}

void UMapSubSystem::LevelClear()
{
	CurClearSpawnerCount++;
	
	if (MinionSpawnerCount > CurClearSpawnerCount) return;

	TArray<AActor*> TPs;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Portal"), TPs);

	SpawnPortal(TPs[0]);
	
	if (!CurMapDatas[CurMapIndex].bIsCleared)
	{
		UpdateCurseQuest(EActiveCurseQuest::ClearMaps, 1);
	}
		
	CurMapDatas[CurMapIndex].bIsCleared = true;
	CurClearSpawnerCount = 0;
}

void UMapSubSystem::AddCurseQuest(FActiveCurseQuest NewQuest)
{
	ActiveCurseQuests.Add(NewQuest);
	OnCurseQuestChanged.Broadcast();
}

void UMapSubSystem::UpdateCurseQuest(EActiveCurseQuest Type, int32 Amount)
{
	if (ActiveCurseQuests.Num() == 0) return;
	
	UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetGameInstance());
	if (!GI || !GI->StatComp) return;
	
	for (int32 i = ActiveCurseQuests.Num() - 1; i >= 0; --i)
	{
		FActiveCurseQuest& Quest = ActiveCurseQuests[i];
		
		if (Quest.QuestType == Type)
		{
			Quest.CurCount += Amount;
			
			if (Quest.CurCount >= Quest.TargetCount)
			{
				FString FloatingTextStr;
				FLinearColor FloatingColor = FLinearColor::White;
				
				switch (Quest.RewardType)
				{
				case ERewardType::RewardGold:
					GI->StatComp->ApplyStatModifier(EStatTypes::Gold, Quest.RewardValue);
					FloatingTextStr = FString::Printf(TEXT("골드 +%d"), Quest.RewardValue);
					FloatingColor = FLinearColor(1.f, 0.617f, 0.f, 1.f);
					break;
				case ERewardType::RewardDreamPowder:
					GI->StatComp->ApplyStatModifier(EStatTypes::DreamDust, Quest.RewardValue);
					FloatingTextStr = FString::Printf(TEXT("꿈가루 +%d"), Quest.RewardValue);
					FloatingColor = FLinearColor(0.053f, 0.510f, 1.0f, 1.f);
					break;
				case ERewardType::RewardMaxHP:
					GI->StatComp->ApplyStatModifier(EStatTypes::MaxHP, Quest.RewardValue);
					GI->HealthComp->Heal(Quest.RewardValue);
					FloatingTextStr = FString::Printf(TEXT("최대 체력 +%d"), Quest.RewardValue);
					FloatingColor = FLinearColor(0.1f, 1.0f, 0.1f, 1.f);
					break;
				case ERewardType::RewardAttackDamage:
					GI->StatComp->ApplyStatModifier(EStatTypes::AttackPower, Quest.RewardValue);
					FloatingTextStr = FString::Printf(TEXT("공격력 +%d"), Quest.RewardValue);
					FloatingColor = FLinearColor(1.0f, 0.1f, 0.1f, 1.f);
					break;
				case ERewardType::RewardMemoryHaste:
					GI->StatComp->ApplyStatModifier(EStatTypes::MemoryAcceleration, Quest.RewardValue);
					FloatingTextStr = FString::Printf(TEXT("기억 가속 +%d"), Quest.RewardValue);
					FloatingColor = FLinearColor(0.8f, 0.2f, 0.8f, 1.f);
					break;
				}
				
				if (GI->FloatingUIActorClass)
				{
					if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
					{
						AFloatingUIActor* FloatingUI = GetWorld()->SpawnActor<AFloatingUIActor>(
							GI->FloatingUIActorClass,
							Player->GetActorLocation(),
							FRotator::ZeroRotator);
						
						if (FloatingUI)
						{
							FloatingUI->ShowFloatingUI(FText::FromString(FloatingTextStr), FloatingColor);
						}
					}
				}
				
				ActiveCurseQuests.RemoveAt(i);
			}
		}
	}
	
	OnCurseQuestChanged.Broadcast();
}
