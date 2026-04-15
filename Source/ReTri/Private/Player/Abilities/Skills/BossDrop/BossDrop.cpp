#include "Player/Abilities/Skills/BossDrop/BossDrop.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Player/Components/AbilityComponent.h"

UBossDrop::UBossDrop()
{
	DataTableRowName = TEXT("boss_drop");
}

bool UBossDrop::CanActivate(ACharacter* Owner)
{
	if (!Owner) return false;

	return Super::CanActivate(Owner);
}

void UBossDrop::Activate(ACharacter* Owner)
{
	if (!Owner) return;

	// 보스 드랍 스킬 발동 시 다음 레벨로 전환
	if (!NextLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(Owner->GetWorld(), NextLevelName);
	}
	else
	{
		UE_LOG(LogTemp,
		       Warning,
		       TEXT("[BossDrop] Activate: NextLevelName이 설정되지 않았습니다. BP_BossDrop의 NextLevelName 속성에 이동할 레벨 이름을 넣어주세요."
		       ));
	}
}
