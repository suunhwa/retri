
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WellSelectUI.generated.h"

class UVerticalBox;
class UWellSelectButtonUI;

UCLASS()
class RETRI_API UWellSelectUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* VerticalBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWellSelectButtonUI> ButtonClass;
	
	UFUNCTION(BlueprintCallable)
	UWellSelectButtonUI* AddButton(FString Description, FString Cost, int32 Index);
	
	UFUNCTION(BlueprintCallable)
	void ClearButtons();
};
