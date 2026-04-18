
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"

class UMiniStatUI;
class USaveSkillSlotUI;
class UPlayDataUI;
class UTextBlock;
class UButton;

struct FPlayerPlayData;

UCLASS()
class RETRI_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	
public:
	UPROPERTY(meta=(BindWidget))
	UMiniStatUI* MiniStatUI;
	
	UPROPERTY(meta=(BindWidget))
	USaveSkillSlotUI* SaveSkillSlot;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreText;
	
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_0;
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_1;
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_2;
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_3;
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_4;
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_5;
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_6;
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_7;
	UPROPERTY(meta=(BindWidget))
	UPlayDataUI* PlayDataUI_8;
	
	UPROPERTY(meta=(BindWidget))
	UButton* RetryButton;
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;
	
	UFUNCTION(BlueprintCallable)
	void SetUIData();
	UFUNCTION(BlueprintCallable)
	void Retry();
	UFUNCTION(BlueprintCallable)
	void Quit();
};
