
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Level/Data/InteractableData.h"
#include "Level/Interfaces/InteractableInterface.h"

#include "InteractableBase.generated.h"

class UCapsuleComponent;
class UWidgetComponent;
class UPaperSpriteComponent;
class UNiagaraComponent;
class USelectUI;

UCLASS()
class RETRI_API AInteractableBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AInteractableBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void Hover_Implementation() override;
	virtual void Interact_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void DataInit(FName InRowName, struct FInteractableData RowData);
	
	UFUNCTION(BlueprintCallable)
	void SetIsUsed(bool IsUsed);
	
	UFUNCTION()
	void UIFocus();
	
	UFUNCTION()
	void UIHold();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* PaperComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* NiagaraComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* OverlapSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* InteractSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* SelectSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* FailedSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractableType InteractableType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	FName MyRowName;
	
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	bool bIsInteractable = false;
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	bool bIsHovering = false;
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	bool bIsUsed = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* InteractUI;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* InteractInfoUI;
	
	float CurDescriptionTime = 0.f;
	float MaxDescriptionTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SelectUI")
	TSubclassOf<UUserWidget> SelectUIClass;

	virtual void ShowSelectUI() {}
	virtual void HideSelectUI() {}
	
protected:
	UPROPERTY() 
	APawn* MyPlayer;
	
	FString InteractName;
	FString Description;
	float Probability;
};
