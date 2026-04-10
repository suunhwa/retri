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
class AItemBase;
class UAbilityBase;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp = 500;
	float CurHp = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dialogue")
	UDataTable* MerchantDialogueDataTable;
	TArray<struct FMerchantDialogueRow*> MerchantDialogue;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	// 상인 일 때
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* MerchantCollision;
	
	// Interaction Object 이름과 상호작용을 띄울 UI (Overlap)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* InteractUI;	
	
	bool bIsInteractable;
	
	//사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* OverlapSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* InteractSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* SelectSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* FailedSound;
	
	// 상점 UI 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShopBGUI* MerchantUIInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShopBGUI> MerchantUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UShopSlotUI> MerchantSlotClass;
	
	void ShowMerchantUI();
	void HideMerchantUI();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemBase> ItemClass;
	
	// SkillID(DataTable Row Key) → ability class 매핑
	// BP에서 획득 스킬(Acquired) 목록 추가해야됩니당
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shop|Skills")
	TMap<FString, TSubclassOf<UAbilityBase>> SkillIDToClassMap;
	
	UFUNCTION()
	void OnClickedMerchantSlotUI(int32 SlotNum);
	
};
