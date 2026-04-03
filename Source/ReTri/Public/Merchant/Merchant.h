// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Level/Interfaces/InteractableInterface.h"
#include "Merchant.generated.h"

class USphereComponent;
class UWidgetComponent;
class UUserWidget;
class UShopBGUI;
class UShopSlotUI;

struct FInteractableData;

UCLASS()
class RETRI_API AMerchant : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMerchant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Interact_Implementation() override;
	
	UFUNCTION()
	void MerchantBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void MerchantEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void DataInit(FName InRowName, FInteractableData RowData);
	
public:
	// 상인 일 때
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* MerchantCollision;
	
	// Interaction Object 이름과 상호작용을 띄울 UI (Overlap)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* InteractUI;	
	
	bool bIsInteractable;
	
	// 상점 UI 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShopBGUI* MerchantUIInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShopBGUI> MerchantUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShopSlotUI> MerchantSlotClass;
	
	void ShowMerchantUI();
	void HideMerchantUI();
	
	UFUNCTION()
	void OnClickedMerchantSlotUI();
};
