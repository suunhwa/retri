
#include "Level/Actors/NZW_TestPlayer.h"
#include "ReTriGameData.h"
#include "Level/Actors/InteractableBase.h"
#include "ReTriGameData.h"

#include "Kismet/KismetSystemLibrary.h"


ANZW_TestPlayer::ANZW_TestPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GD = CreateDefaultSubobject<UReTriGameData>(TEXT("GameData"));
}

void ANZW_TestPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANZW_TestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANZW_TestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANZW_TestPlayer::HoverInteractable()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); // Interaction
	
	FHitResult HitResult;
	bool bHit = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(ObjectTypes, false, HitResult);
	if (bHit && HitResult.GetActor()->Implements<UInteractableInterface>())
	{
		IInteractableInterface::Execute_Hover(HitResult.GetActor());
	}
}

void ANZW_TestPlayer::Interaction()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); // Interaction
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	TArray<AActor*> OutActors;
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 42.f, ObjectTypes, AActor::StaticClass(), IgnoreActors,OutActors);
	
	if (!bHit) return;
	for (AActor* Actor : OutActors)
	{
		AInteractableBase* Interact = Cast<AInteractableBase>(Actor);
		if (Interact)
			IInteractableInterface::Execute_Interact(Interact);
	}
}

