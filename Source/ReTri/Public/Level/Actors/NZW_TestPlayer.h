
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NZW_TestPlayer.generated.h"

UCLASS()
class RETRI_API ANZW_TestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ANZW_TestPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void HoverInteractable();
	
	UFUNCTION(BlueprintCallable)
	void Interaction();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UReTriGameData* GD;
};
