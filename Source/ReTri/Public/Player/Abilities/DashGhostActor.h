// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DashGhostActor.generated.h"

class UPoseableMeshComponent;
class UMaterialInstanceDynamic;
class USkeletalMeshComponent;

/**
 * 대시 잔상 Actor
 * - 캐릭터 스켈레탈 메시 포즈 스냅샷을 찍어 페이드아웃
 * - BP에서 GhostMaterial에 Opacity / Color 파라미터가 있는 반투명 머티리얼 할당 필요
 */
UCLASS()
class RETRI_API ADashGhostActor : public AActor
{
	GENERATED_BODY()

public:
	ADashGhostActor();

	// 스폰 직후 호출 — 포즈 복사 + 머티리얼 세팅
	void Init(USkeletalMeshComponent* SourceMesh);

protected:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPoseableMeshComponent> GhostMesh;

	// 반투명 머티리얼 (Opacity, Color 스칼라/벡터 파라미터 필요)
	UPROPERTY(EditDefaultsOnly, Category="Ghost")
	TObjectPtr<UMaterialInterface> GhostMaterial;

	// 잔상 색상 틴트 (기본: 파란빛)
	UPROPERTY(EditDefaultsOnly, Category="Ghost")
	FLinearColor GhostColor = FLinearColor(0.2f, 0.6f, 1.f, 1.f);

	// 페이드아웃 총 시간 (초)
	UPROPERTY(EditDefaultsOnly, Category="Ghost")
	float FadeDuration = 0.2f;

	// 시작 시 최대 투명도
	UPROPERTY(EditDefaultsOnly, Category="Ghost")
	float StartOpacity = 0.3f;

	float ElapsedTime = 0.f;
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;
};
