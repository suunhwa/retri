// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Level/Data/InteractableData.h"
#include "Level/Interfaces/InteractableInterface.h"

#include "InteractableBase.generated.h"

class UCapsuleComponent;
class UWidgetComponent;
class UPaperSpriteComponent;
class USelectUI;

UCLASS()
class RETRI_API AInteractableBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void Hover_Implementation() override;
	virtual void Interact_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void DataInit(FName InRowName, struct FInteractableData RowData);
	
	UFUNCTION(BlueprintCallable)
	void SetIsUsed(bool IsUsed);
	
	UFUNCTION()
	void UIFocus();
	
	UFUNCTION()
	void UIHold();
	
//! 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* PaperComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractableType InteractableType;
	//FInteractableData InteractableData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	FName MyRowName;
	
	// 상호작용 중인지
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	bool bIsInteractable = false;
	// 마우스로 가리키고 있는지
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	bool bIsHovering = false;
	// 사용 했는지
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	bool bIsUsed = false;
	
	// Interaction Object 이름과 상호작용을 띄울 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* InteractUI;	
	// Interaction Object 정보를 띄울 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* InteractInfoUI;
	
	float CurDescriptionTime = 0.f;
	float MaxDescriptionTime = 3.f;

	// === [ Select ] ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SelectUI")
	TSubclassOf<USelectUI> SelectUIClass;
	UPROPERTY()
	USelectUI* SelectUIInstance;
	
	void ShowSelectUI();
	void HideSelectUI();
	
protected:
	UPROPERTY() //todo 플레이어 클래스로 바꿔야할수도 
	APawn* MyPlayer;
	
	FString InteractName;
	FString Description;
	float Probability;
	
};
