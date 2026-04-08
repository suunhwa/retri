// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveSkillSlotUI.generated.h"

class USkillSlotUI;

UCLASS()
class RETRI_API USaveSkillSlotUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	USkillSlotUI* SkillSlotUI_0;
	UPROPERTY(meta=(BindWidget))
	USkillSlotUI* SkillSlotUI_1;
	UPROPERTY(meta=(BindWidget))
	USkillSlotUI* SkillSlotUI_2;
	UPROPERTY(meta=(BindWidget))
	USkillSlotUI* SkillSlotUI_3;
};
