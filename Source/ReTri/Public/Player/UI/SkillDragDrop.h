// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SkillDragDrop.generated.h"

class USkillSlotUI;

/**
 *  skill slot drag & drop payload
 *  NativeOnDragDetected에서 생성, NativeOnDrop에서 수신
 */
UCLASS()
class RETRI_API USkillDragDrop : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	// drag한 slot
	UPROPERTY(BlueprintReadWrite, Category="SkillDrag")
	TObjectPtr<USkillSlotUI> SourceSlot;
};
