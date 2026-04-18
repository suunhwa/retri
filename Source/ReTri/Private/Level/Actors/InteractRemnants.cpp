

#include "Level/Actors/InteractRemnants.h"

#include "Item/ItemBase.h"
#include "MapSubSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"


AInteractRemnants::AInteractRemnants()
{
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNiagara(TEXT("/Game/Free_Magic/VFX_Niagara/NS_Remnants.NS_Remnants"));
	if (TempNiagara.Succeeded()) NiagaraComp->SetAsset(TempNiagara.Object);
	NiagaraComp->SetRelativeLocation(FVector(0.0f, 0.0f, -180.0f));
	NiagaraComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	NiagaraComp->bAutoActivate = false;
}

void AInteractRemnants::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractRemnants::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	FName KeyName = FName("Remnants");
	bool* FoundValue = GetGameInstance()->GetSubsystem<UMapSubSystem>()->GetCurMapData().SpawnInteractableRowNames.Find(KeyName);
	if (FoundValue) *FoundValue = true; 
	SetIsUsed(true);
	
	if (!ItemClass) return;
	
	auto GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GI) return;
	auto MapSub = GI->GetSubsystem<UMapSubSystem>();
	if (!MapSub) return;
	
	TArray<FPlayerSkillData*> SkillRandomDatas = MapSub->GetRandomAcquiredItemList();
	int32 RandomNum = FMath::RandRange(0, SkillRandomDatas.Num()-1);
	
	FVector Loc = GetActorLocation() + (GetActorRightVector() * 300.f);
	
	FPlayerSkillData* PickedData = SkillRandomDatas[RandomNum];
	auto* Item = GetWorld()->SpawnActor<AItemBase>(ItemClass, Loc, FRotator::ZeroRotator);
	if (Item)
	{
		Item->DataInit(*PickedData);

		FName RowName = FName(*PickedData->SkillID);
		if (TSubclassOf<UAbilityBase>* Found = SkillIDToClassMap.Find(RowName))
		{
			Item->AbilityClass = *Found;
			UGameplayStatics::PlaySound2D(GetWorld(), SelectSound);
		}
	}
	
	NiagaraComp->Activate();
}
