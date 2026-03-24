// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ReTriPlayerController.h"


AReTriPlayerController::AReTriPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AReTriPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

bool AReTriPlayerController::GetMouseWorldPosition(FVector& OutPosition) const
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return false;

	FVector WorldLocation, WorldDirection;
	if (!DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) return false;

	if (FMath::IsNearlyZero(WorldDirection.Z)) return false;

	float T = (ControlledPawn->GetActorLocation().Z - WorldLocation.Z) / WorldDirection.Z;
	OutPosition = WorldLocation + WorldDirection * T;
	return true;
}