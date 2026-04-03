// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/Data/PlayerSkillData.h"
#include "ItemBase.generated.h"

class USphereComponent;
class UWidgetComponent;
class UUserWidget;
struct FPlayerSkillData;

UCLASS()
class RETRI_API AItemBase : public AActor
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
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* WidgetComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetClass;
	
	FPlayerSkillData CurSkillData;
	void DataInit(FPlayerSkillData SkillData);
};
