

#include "ReTriGameData.h"

UReTriGameData::UReTriGameData()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UReTriGameData::BeginPlay()
{
	Super::BeginPlay();

}

void UReTriGameData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
