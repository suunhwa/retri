// Fill out your copyright notice in the Description page of Project Settings.

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
	// BP_BossDrop 블루프린트의 NextLevelName 속성에 레벨 이름을 지정해 주세요.
	if (!NextLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(Owner->GetWorld(), NextLevelName);
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("[BossDrop] Activate: NextLevelName이 설정되지 않았습니다. BP_BossDrop의 NextLevelName 속성에 이동할 레벨 이름을 넣어주세요."));
	}
	
	// 주의: UAbilityBase::Activate는 PURE_VIRTUAL이므로 Super::Activate() 호출 금지!

	
	//
	// APlayerCharacter* Player = Cast<APlayerCharacter>(Owner);
	// if (!Player) return;
	//
	// UAbilityComponent* AbilityComp = Player->GetAbilityComponent();
	// if (!AbilityComp) return;
	//
	// if (!AbilityComp->EquipAcquiredSkill(GetClass()))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("[BossDrop] 빈 슬롯 없음"));
	// }
	//
	// // // TODO: 레벨 이름 넣으면 됩니당
	// // if (!NextLevelName.IsNone())
	// //     UGameplayStatics::OpenLevel(Owner->GetWorld(), NextLevelName);

	// Super::Activate(Owner);
}
