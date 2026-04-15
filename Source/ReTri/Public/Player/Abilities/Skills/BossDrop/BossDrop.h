#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/AbilityBase.h"
#include "BossDrop.generated.h"

/**
 * 보스 사망 시 드랍되는 스킬 (CSV ID: boss_drop)
 * 다른 스킬들과 동일하게 Q/E 슬롯에 장착되며,
 * 활성화 시 SkillToGive를 플레이어 슬롯에 장착
 */
UCLASS()
class RETRI_API UBossDrop : public UAbilityBase
{
	GENERATED_BODY()

public:
	UBossDrop();

protected:
	virtual bool CanActivate(ACharacter* Owner) override;
	virtual void Activate(ACharacter* Owner) override;

public:
	// 전환할 레벨 이름
	UPROPERTY(EditDefaultsOnly, Category="BossDrop")
	FName NextLevelName = NAME_None;

	// TODO: 스킬 장착 방식으로 바꿀 경우 사용
	UPROPERTY(EditDefaultsOnly, Category="BossDrop")
	TSubclassOf<UAbilityBase> SkillToGive;
};
