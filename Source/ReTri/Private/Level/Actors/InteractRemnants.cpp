// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractRemnants.h"

#include "Item/ItemBase.h"
#include "MapSubSystem.h"
#include "Kismet/GameplayStatics.h"


void AInteractRemnants::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Remnants;
}

void AInteractRemnants::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	FName KeyName = FName("Remnants");
	bool* FoundValue = GetGameInstance()->GetSubsystem<UMapSubSystem>()->GetCurMapData().SpawnInteractableRowNames.Find(KeyName);
	if (FoundValue) *FoundValue = true; 
	SetIsUsed(true);
	
	SCREENLOG("스킬을 랜덤으로 스폰");
	if (!ItemClass)
	{
		UE_LOG(jiwon, Error, TEXT("ItemClass 설정안됨 ㄷㄷ"));
		return;
	}
	
	if (auto GI = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		if (auto MapSub = GI->GetSubsystem<UMapSubSystem>())
		{
			UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
			
			TArray<FPlayerSkillData*> SkillRandomDatas = MapSub->GetRandomAcquiredItemList();
			int32 RandomNum = FMath::RandRange(0, SkillRandomDatas.Num()-1);
			
			FVector Loc = GetActorLocation() + (GetActorRightVector() * 300.f);
			auto* Item = GetWorld()->SpawnActor<AItemBase>(ItemClass, Loc, FRotator::ZeroRotator);
			if (Item)
			{
				Item->DataInit(*SkillRandomDatas[RandomNum]);
			}
			else
			{
				SCREENLOG("아이템 스폰 실패!");
			}
		}
	}
	
}
