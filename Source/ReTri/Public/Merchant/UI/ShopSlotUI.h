
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopSlotUI.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillSlotClicked, int32, SlotNum);

UCLASS()
class RETRI_API UShopSlotUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnSkillSlotClicked OnSlotClicked;
	
	UFUNCTION()
	void OnClicked();
	UFUNCTION()
	void OnHover();
	
	UFUNCTION(BlueprintCallable)
	void SetGoldText(int32 Gold, FLinearColor Color = FLinearColor(0.8f, 0.8f, 0.5f, 1.0f));

	UFUNCTION(BlueprintCallable)
	void SetItemIcon(UTexture2D* Texture);
	
	UPROPERTY(meta=(BindWidget))
	UButton* ItemSlotButton;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* GoldText;
	
	int32 SlotNumber;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* HoverSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* ClickSound;
};
