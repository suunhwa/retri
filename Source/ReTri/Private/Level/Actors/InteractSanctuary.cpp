// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractSanctuary.h"

#include "MapSubSystem.h"
#include "ReTriGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Level/Actors/FloatingUIActor.h"
#include "Player/Components/HealthComponent.h"
#include "Player/Components/StatComponent.h"
#include "NiagaraComponent.h"

AInteractSanctuary::AInteractSanctuary()
{
	// ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/Free_Magic/VFX_Niagara/NS_Sanctuary.NS_Sanctuary'"));
	// if (TempNiagara.Succeeded()) NiagaraComp->SetAsset(TempNiagara.Object);
	// NiagaraComp->SetRelativeLocation(FVector(0.0f, 50.0f, 80.0f));
	// NiagaraComp->bAutoActivate = false;
	
	// HealStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealStaticMesh"));
	// HealStaticMesh->SetupAttachment(RootComponent);
	// ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelInteraction/03_Assets/Mesh/DreamPowder_Test.DreamPowder_Test''"));
	// if (TempMesh.Succeeded()) HealStaticMesh->SetStaticMesh(TempMesh.Object);
	// HealStaticMesh->SetRelativeLocation(FVector(0.0f, 50.0f, 40.0f));
}

void AInteractSanctuary::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableType = EInteractableType::Sanctuary;
}

void AInteractSanctuary::Interact_Implementation()
{
	// Super::Interact_Implementation();
	if (!MyPlayer) return;
	
	auto GI = Cast<UReTriGameInstance>(GetGameInstance());

	FString FloatingText = FString::Printf(TEXT("골드 부족!"));
	FLinearColor FloatingColor = FLinearColor::Red;
	
	if (!GI->StatComp->SpendGold(Cost))
	{
		if (GI->FloatingUIActorClass)
		{
			if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				AFloatingUIActor* FloatingUIActor = GetWorld()->SpawnActor<AFloatingUIActor>(
					GI->FloatingUIActorClass, Player->GetActorLocation(), FRotator::ZeroRotator);
				FloatingUIActor->ShowFloatingUI(FText::FromString(FloatingText), FloatingColor);
			}
		}
		
		return;
	}
	
	GI->HealthComp->Heal(GI->HealthComp->GetMaxHP());
	
	FloatingText = FString::Printf(TEXT("%d"), FMath::TruncToInt(GI->HealthComp->GetMaxHP())); 
	FloatingColor = FLinearColor(1.0f, 0.5f, 0.5f, 1.f);
	
	FName KeyName = FName("Sanctuary");
	bool* FoundValue = GetGameInstance()->GetSubsystem<UMapSubSystem>()->GetCurMapData().SpawnInteractableRowNames.Find(KeyName);
	if (FoundValue) *FoundValue = true; 
	
	if (GI->FloatingUIActorClass)
	{
		if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			AFloatingUIActor* FloatingUIActor = GetWorld()->SpawnActor<AFloatingUIActor>(
				GI->FloatingUIActorClass, Player->GetActorLocation(), FRotator::ZeroRotator);
			FloatingUIActor->ShowFloatingUI(FText::FromString(FloatingText), FloatingColor);
		}
	}
	
	SetIsUsed(true);
	
	// UE_LOG(jiwon, Warning, TEXT("%s"), *InteractName);
	Super::Interact_Implementation();
}
