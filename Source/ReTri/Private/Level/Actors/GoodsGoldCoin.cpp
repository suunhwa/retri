// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsGoldCoin.h"

#include "ReTriGameData.h"
#include "ReTriGameInstance.h"
#include "Level/Actors/FloatingUIActor.h"

#include "Player/Components/StatComponent.h"

AGoodsGoldCoin::AGoodsGoldCoin()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/low-poly_gold_coin/StaticMeshes/SM_Gold.SM_Gold'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void AGoodsGoldCoin::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor == TargetPlayer)
	{
		auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());

		if (!GI || !GI->StatComp) return;
		const int32 Amount = GI->GameData->GetRandomGold();
		GI->StatComp->ApplyStatModifier(EStatTypes::Gold, Amount);
		
		if (!FloatingUIActorClass) return;
		AFloatingUIActor* FloatingUI = GetWorld()->SpawnActor<AFloatingUIActor>(FloatingUIActorClass, OtherActor->GetActorLocation(), FRotator::ZeroRotator);
		FString TempStr = FString::Printf(TEXT("골드 +%d"), Amount);
		FloatingUI->ShowFloatingUI(FText::FromString(TempStr), FLinearColor(1.f, 0.617f, 0.f, 1.f));
		
		GI->DebugStat();
		Destroy();
	}
}
