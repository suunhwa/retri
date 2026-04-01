// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsDreamPowder.h"

#include "ReTriGameData.h"
#include "ReTriGameInstance.h"
#include "BehaviorTree/Blackboard/BlackboardKeyEnums.h"
#include "Level/Actors/FloatingUIActor.h"

AGoodsDreamPowder::AGoodsDreamPowder()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/gold-star/source/SM_Dream.SM_Dream'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);  
}

void AGoodsDreamPowder::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoodsDreamPowder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoodsDreamPowder::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor == TargetPlayer)
	{
		auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
		int32 CurDreamPowder = GI->GameData->GetRandomDreamPowder();
		GI->GameData->UpdateDreamPowder(+CurDreamPowder);
		//todo 선화가 만든 Stat으로 바꿔야함 GI->CurPlayerStat.DreamPowder += GI->GameData->GetRandomDreamPowder();
		
		if (FloatingUIActorClass)
		{
			AFloatingUIActor* FloatingUI = GetWorld()->SpawnActor<AFloatingUIActor>(FloatingUIActorClass, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
			FString TempStr = FString::Printf(TEXT("꿈가루 +%d"), CurDreamPowder);
			FloatingUI->ShowFloatingUI(FText::FromString(TempStr), FLinearColor(0.053033f, 0.510102f, 1.0f, 1.f));
		}
		
		GI->GameData->DebugStat();
		Destroy();
	}
}
