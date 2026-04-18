
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableUI.generated.h"

class UTextBlock; 

UCLASS()
class RETRI_API UInteractableUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnInteract(const FString& Name);
	
protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnInteract"))
	void BP_Interact_UI(const FString& Name);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* InteractableName;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Big;
	
	void PlayBigAnimation();
	void ReverseBigAnimation();
};
