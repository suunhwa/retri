
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SkillItemInterface.h"
#include "Player/Abilities/AbilityBase.h"
#include "Player/Data/PlayerSkillData.h"
#include "ItemBase.generated.h"

class USphereComponent;
class UWidgetComponent;
class UDropItemUI;
class USoundBase;
struct FPlayerSkillData;

UCLASS()
class RETRI_API AItemBase : public AActor, public ISkillItemInterface
{
	GENERATED_BODY()
	
public:	
	AItemBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void Acquire_Implementation() override;
	virtual void Hold_Implementation(AActor* PlayerActor) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Collision")
	USphereComponent* SphereComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Collision")
	UWidgetComponent* ItemUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AGoodsBase> GoodsClass;
	
	FPlayerSkillData CurSkillData;
	FPlayerSkillData GetSkillData() { return CurSkillData; }
	void DataInit(FPlayerSkillData SkillData);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skill")
	TSubclassOf<UAbilityBase> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	TObjectPtr<USoundBase> EquipSuccessSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	TObjectPtr<USoundBase> EquipFullSound;
	
	void SetInteractionUIVisibility(bool isVisible);
};
