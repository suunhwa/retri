
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingUIActor.generated.h"

class UWidgetComponent;

UCLASS()
class RETRI_API AFloatingUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloatingUIActor();

	void ShowScaleUI(FText Text, FLinearColor Color);
	
	void ShowFloatingUI(FText Text, FLinearColor Color);
	
	void ShowFloatingLongUI(FText Text, FLinearColor Color);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* WidgetComp;
};
