// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/GoodsExp.h"

#include "ReTriGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Level/Actors/FloatingUIActor.h"
#include "Player/PlayerCharacter.h"

#include "Player/Components/StatComponent.h"


AGoodsExp::AGoodsExp()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/GoodsExp.GoodsExp'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void AGoodsExp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!OtherActor->IsA(APlayerCharacter::StaticClass())) return; 

	auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
	if (!GI || !GI->StatComp) return;
	
	GI->StatComp->AddExp(Amount);
	
	if (!FloatingUIActorClass) return;
	AFloatingUIActor* FloatingUI = GetWorld()->SpawnActor<AFloatingUIActor>(FloatingUIActorClass, OtherActor->GetActorLocation(), FRotator::ZeroRotator);
	FString TempStr = FString::Printf(TEXT("EXP +%d"), Amount);
	FloatingUI->ShowFloatingUI(FText::FromString(TempStr), FLinearColor(0.4f, 1.0f, 0.3f, 1.f));

	GI->DebugStat();
	
	UGameplayStatics::PlaySound2D(GetWorld(), AcquireSound, 0.5f);

	Destroy();
}