
#include "Level/Actors/BossDropItem.h"

ABossDropItem::ABossDropItem()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
}
