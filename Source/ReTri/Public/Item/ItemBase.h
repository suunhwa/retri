// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SkillItemInterface.h"
#include "Player/Data/PlayerSkillData.h"
#include "ItemBase.generated.h"

class USphereComponent;
class UWidgetComponent;
class UDropItemUI;
struct FPlayerSkillData;

UCLASS()
class RETRI_API AItemBase : public AActor, public ISkillItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void Acquire_Implementation() override;
	virtual void Hold_Implementation(AActor* PlayerActor) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Collision")
	USphereComponent* SphereComp;
	
	// Item Image와 상호작용을 띄울 UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Collision")
	UWidgetComponent* ItemUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AGoodsBase> GoodsClass;
	
	FPlayerSkillData CurSkillData;
	FPlayerSkillData GetSkillData() { return CurSkillData; }
	void DataInit(FPlayerSkillData SkillData);
	
	void SetInteractionUIVisibility(bool isVisible);
};
