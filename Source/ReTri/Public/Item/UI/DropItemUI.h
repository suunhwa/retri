
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DropItemUI.generated.h"

class UImage;
class UCanvasPanel;
class UTextBlock;
class UProgressBar;

UCLASS()
class RETRI_API UDropItemUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* ItemInteractCanvas;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(meta=(BindWidget))
	UProgressBar* DecomposeBar;
	
	UFUNCTION(BlueprintCallable)
	void SetItemIcon(UTexture2D* Texture);
	UFUNCTION(BlueprintCallable)
	void SetItemName(FString Text);
	UFUNCTION(BlueprintCallable)
	void SetDecomposeBar(float CurDeltaTime);
	
	void SetItemInteractCanvasVisibility(ESlateVisibility Visible);
};
