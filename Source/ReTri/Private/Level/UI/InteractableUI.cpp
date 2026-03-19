// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/UI/InteractableUI.h"

void UInteractableUI::OnInteract(const FString& Name)
{
	BP_Interact_UI(Name);
}
