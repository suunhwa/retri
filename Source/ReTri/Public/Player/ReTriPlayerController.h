#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ReTriPlayerController.generated.h"


UCLASS()
class RETRI_API AReTriPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AReTriPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	// 마우스 커서 위치를 플레이어 Z 평면과 교점으로 반환
	bool GetMouseWorldPosition(FVector& OutPosition) const;
};
