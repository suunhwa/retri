

#include "Level/Actors/LootDreamPowderPillar.h"

#include "Kismet/GameplayStatics.h"
#include "Level/Actors/GoodsDreamPowder.h"

ALootDreamPowderPillar::ALootDreamPowderPillar()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Stylized_PBR_Nature/Rocks/Assets/SM_S_Rock_04.SM_S_Rock_04'"));
	if (TempMesh.Succeeded()) MeshComp->SetStaticMesh(TempMesh.Object);
}

void ALootDreamPowderPillar::Break()
{
	Super::Break();
	
	UGameplayStatics::PlaySound2D(GetWorld(), BreakSound2);
	
	GetWorld()->SpawnActor<AGoodsDreamPowder>(GoodsClass, GetActorLocation(), GetActorRotation());
	
}
