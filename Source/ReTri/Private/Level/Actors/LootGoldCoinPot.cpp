

#include "Level/Actors/LootGoldCoinPot.h"

#include "Kismet/GameplayStatics.h"
#include "Level/Actors/GoodsGoldCoin.h"

ALootGoldCoinPot::ALootGoldCoinPot()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Stylized_PBR_Nature/Rocks/Assets/SM_S_Rock_04.SM_S_Rock_04'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void ALootGoldCoinPot::Break()
{
	Super::Break();

	UGameplayStatics::PlaySound2D(GetWorld(), BreakSound1);
	
	GetWorld()->SpawnActor<AGoodsGoldCoin>(GoodsClass, GetActorLocation(), GetActorRotation());
}
