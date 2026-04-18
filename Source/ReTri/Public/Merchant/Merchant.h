
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
	AMerchant();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	virtual void Tick(float DeltaTime) override;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* MerchantCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* InteractUI;	
	
	bool bIsInteractable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* OverlapSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* InteractSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* SelectSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* FailedSound;
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shop|Skills")
	TMap<FString, TSubclassOf<UAbilityBase>> SkillIDToClassMap;
	
	UFUNCTION()
	void OnClickedMerchantSlotUI(int32 SlotNum);
};
