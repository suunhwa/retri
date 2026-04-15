// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/DarkMoon/DarkMoonAnimInstance.h"


void UDarkMoonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	APawn* Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		FVector Velocity = Pawn->GetVelocity();
		
		Velocity.Z = 0;
		CurrentSpeed = Velocity.Size();
	}
}