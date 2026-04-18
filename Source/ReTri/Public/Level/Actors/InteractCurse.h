
#pragma once

#include "CoreMinimal.h"
#include "Level/Actors/InteractableBase.h"
#include "Level/Data/InteractableData.h"
#include "InteractCurse.generated.h"

UCLASS()
class RETRI_API AInteractCurse : public AInteractableBase
{
	GENERATED_BODY()

protected:
	AInteractCurse();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:	
	virtual void Interact_Implementation() override;
	
	UFUNCTION()
	void OnCurseSelected(int32 Index);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Curse")
	UDataTable* CurseDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Curse")
	UDataTable* CurseRewardDataTable;
	
	UPROPERTY()
	USelectUI* SelectUIInstance;
	
	virtual void ShowSelectUI() override;
	virtual void HideSelectUI() override;
	
private:
	TArray<FCurseData*> CurseDatas;
	TArray<FCurseRewardData*> CurseRewardDatas;
};