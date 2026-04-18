
#pragma once

#include "CoreMinimal.h"
#include "ReTri/ReTri.h"
#include "GameFramework/Actor.h"
#include "GoodsBase.generated.h"

class USphereComponent;
class UFloatingUI;

UCLASS()
class RETRI_API AGoodsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoodsBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION(BlueprintCallable)
	void MoveAndRotate(float DeltaTime);
	
protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere)
	AActor* TargetPlayer;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YawRotSpeed = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollRotSpeed = 1500.f;
	
	bool bIsFixedAmount = false;
	int32 Amount = 0;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class AFloatingUIActor> FloatingUIActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* AcquireSound;
	
};
