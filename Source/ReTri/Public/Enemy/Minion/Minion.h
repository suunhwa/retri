// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Minion.generated.h"


UCLASS()
class RETRI_API AMinion : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	AMinion();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 플레이어
	UPROPERTY()
	class ACharacter* PlayerTarget;
	
	// 미니언 공격 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAttacking = false;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	class UMyMinionFSM* FSM;
	
	// === EXP Drop ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpDrop")
	int32 AmountExp = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpDrop")
	TSubclassOf<class AGoodsBase> GoodsEXPClass;
	
	// === Minimap ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	class UPaperSpriteComponent* PaperComp;
	
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	FEnemyDataTableRow* GetStatInfo() { return MyStatInfo; }
	
	virtual void OnAttackOverlap(AActor* OtherActor) override;
	void DoRagdoll();
	void PlayMontage(UAnimMontage* MontageToPlay);

public:
	// === Hit Sounds (랜덤 재생) ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<class USoundBase*> HitSounds;
};
