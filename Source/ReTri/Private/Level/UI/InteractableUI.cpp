

#include "Level/UI/InteractableUI.h"

void UInteractableUI::OnInteract(const FString& Name)
{
	BP_Interact_UI(Name);
}

void UInteractableUI::PlayBigAnimation()
{
	PlayAnimation(Big);
}

void UInteractableUI::ReverseBigAnimation()
{
	PlayAnimation(Big, 0, 1, EUMGSequencePlayMode::Reverse);
}
