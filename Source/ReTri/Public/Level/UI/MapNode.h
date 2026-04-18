
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MapNode.generated.h"

class UImage;
class UButton;

UCLASS()
class RETRI_API UMapNode : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* CurUI;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Sanctuary;
	UFUNCTION(BlueprintCallable)
	void SetSanctuaryVisibility(ESlateVisibility InVisibility){ Sanctuary->SetVisibility(InVisibility); }
	
	UPROPERTY(meta = (BindWidget))
	UImage* Well;
	UFUNCTION(BlueprintCallable)
	void SetWellVisibility(ESlateVisibility InVisibility) { Well->SetVisibility(InVisibility); }
	
	UPROPERTY(meta = (BindWidget))
	UButton* Node;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Hover;

	UFUNCTION(BlueprintCallable)
	void OnNodeHover();
	UFUNCTION(BlueprintCallable)
	void OnNodeHoverEnd();
	UFUNCTION(BlueprintCallable)
	void OnNodeClicked();
	
	UPROPERTY(BlueprintReadOnly)
	int32 NodeIndexNumber; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* HoverSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* ClickSound;
};
