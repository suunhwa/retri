
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "LootBase.generated.h"

class UCapsuleComponent;

UCLASS()
class RETRI_API ALootBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ALootBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Break();
	
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* DamageSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* BreakSound1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	USoundBase* BreakSound2;

	UPROPERTY()
	bool bIsBroken = false;
	
	UPROPERTY()
	int32 HP = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AGoodsBase> GoodsClass;
};
