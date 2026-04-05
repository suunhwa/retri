// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Abilities/DashGhostActor.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ADashGhostActor::ADashGhostActor()
{
	PrimaryActorTick.bCanEverTick = true;

	GhostMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("GhostMesh"));
	RootComponent = GhostMesh;
}

void ADashGhostActor::Init(USkeletalMeshComponent* SourceMesh)
{
	if (!SourceMesh) return;

	// 포즈 스냅샷 복사
	GhostMesh->SetSkinnedAssetAndUpdate(SourceMesh->GetSkeletalMeshAsset());
	GhostMesh->SetWorldTransform(SourceMesh->GetComponentTransform());
	GhostMesh->CopyPoseFromSkeletalComponent(SourceMesh);

	// 머티리얼 동적 인스턴스 생성 (슬롯별)
	if (GhostMaterial)
	{
		const int32 MatCount = GhostMesh->GetNumMaterials();
		for (int32 i = 0; i < MatCount; i++)
		{
			UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(GhostMaterial, this);
			DynMat->SetVectorParameterValue(TEXT("Color"), GhostColor);
			DynMat->SetScalarParameterValue(TEXT("Opacity"), StartOpacity);
			GhostMesh->SetMaterial(i, DynMat);
			DynamicMaterials.Add(DynMat);
		}
	}

	SetLifeSpan(FadeDuration + 0.1f);
}

void ADashGhostActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	const float Alpha = FMath::Clamp(1.f - (ElapsedTime / FadeDuration), 0.f, 1.f);
	const float CurrentOpacity = StartOpacity * Alpha;

	for (UMaterialInstanceDynamic* DynMat : DynamicMaterials)
	{
		if (DynMat)
		{
			DynMat->SetScalarParameterValue(TEXT("Opacity"), CurrentOpacity);
		}
	}

	if (Alpha <= 0.f)
	{
		Destroy();
	}
}
