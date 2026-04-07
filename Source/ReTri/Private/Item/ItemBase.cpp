// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"
#include "Item/UI/DropItemUI.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Level/Actors/GoodsDreamPowder.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Player/PlayerCharacter.h"
#include "Player/Components/AbilityComponent.h"
#include "Player/Components/StatComponent.h"
#include "ReTri/ReTri.h"

class UInteractableUI;
// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(150.f);
	SphereComp->SetCollisionObjectType(ECC_GameTraceChannel6); // SkillItem
	// SphereComp->SetCollisionProfileName(TEXT("OverlapAll"));
	
	ItemUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	ItemUI->SetupAttachment(SphereComp);
	ItemUI->SetWidgetSpace(EWidgetSpace::World); 
	ConstructorHelpers::FClassFinder<UDropItemUI> TempUI(TEXT("/Game/Item/UI/WBP_DropItemUI.WBP_DropItemUI_C"));
	if (TempUI.Succeeded()) ItemUI->SetWidgetClass(TempUI.Class);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	// SphereComp->SetSimulatePhysics(true);
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (CamManager)
	{
		// UI 방향을 카메라를 바라볼 수 있도록
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(ItemUI->GetComponentLocation(), CamManager->GetCameraLocation());
		ItemUI->SetWorldRotation(LookAtRot);
	}
}

void AItemBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		SetInteractionUIVisibility(true);
	}
}

void AItemBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		SetInteractionUIVisibility(false);
	}
}

void AItemBase::Acquire_Implementation()
{
	ISkillItemInterface::Acquire_Implementation();
	
	/*// todo F 습득 행동 
	SCREENLOG("습득하는 경우와 인벤토리가 가득 차있는 경우");*/
	
	if (!AbilityClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemBase::Acquire — AbilityClass 미설정"));
		return;
	}

	APlayerCharacter* Player = Cast<APlayerCharacter>(
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player) return;

	UAbilityComponent* AbilityComp = Player->GetAbilityComponent();
	if (!AbilityComp) return;

	// 빈 슬롯 있으면 장착
	if (AbilityComp->EquipAcquiredSkill(AbilityClass))
	{
		if (EquipSuccessSound)
			UGameplayStatics::PlaySound2D(GetWorld(), EquipSuccessSound);

		Destroy();
	}
	else
	{
		// 모두 꽉 참 — 사운드만 재생
		// 플레이어 쪽에서 스킬 버리고 다시 픽업
		if (EquipFullSound)
			UGameplayStatics::PlaySound2D(GetWorld(), EquipFullSound);
	}
}

void AItemBase::Hold_Implementation(AActor* PlayerActor)
{
	ISkillItemInterface::Hold_Implementation(PlayerActor);
	
	if (!PlayerActor->IsA(APlayerCharacter::StaticClass())) return;
	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerActor);
	
	//if (Player->CurHoldTime < Player->MaxHoldTime) return;
	
	// stat에 바로 꿈가루 반영해주기 위해 가져옴
	UStatComponent* StatComp = Player->GetStatComponent();
	if (!StatComp) return;
	
	/*SCREENLOG("분해 완료!");
	
	AGoodsDreamPowder* Goods = GetWorld()->SpawnActor<AGoodsDreamPowder>(GoodsClass, GetActorLocation(), GetActorRotation());
	Goods->bIsFixedAmount = true;
	Goods->Amount = CurSkillData.UpgradeCostDreamDust;*/
	
	// CSV의 UpgradeCostDreamDust 값을 분해 보상으로 지급
	int32 RewardAmount = CurSkillData.UpgradeCostDreamDust;
	UE_LOG(LogTemp, Warning, TEXT("[Salvage] 꿈가루: %d"), RewardAmount);
	if (RewardAmount <= 0) return;

	StatComp->ApplyStatModifier(EStatTypes::DreamDust, static_cast<float>(RewardAmount));
	UE_LOG(LogTemp, Warning, TEXT("[Salvage] 꿈가루 지급 성공: %d"), RewardAmount);
	
	Destroy();
}

void AItemBase::DataInit(FPlayerSkillData SkillData)
{
	CurSkillData = SkillData;
	
	if (!ItemUI)
	{
		UE_LOG(jiwon, Error, TEXT("ItemUI 설정안됨 ㄷㄷ"));
		return;
	}
	
	UDropItemUI* DropItem = Cast<UDropItemUI>(ItemUI->GetUserWidgetObject());
	if (!DropItem) return;
	DropItem->SetItemIcon(SkillData.Icon);
	DropItem->SetItemName(SkillData.SkillNameKR);
	DropItem->SetDecomposeBar(0.0f);
}

void AItemBase::SetInteractionUIVisibility(bool isVisible)
{
	UDropItemUI* DropItem = Cast<UDropItemUI>(ItemUI->GetUserWidgetObject());
	if (!DropItem) 
	{
		UE_LOG(jiwon, Error, TEXT("DropItem 설정안됨 ㄷㄷ"));
		return;
	}
	
	if (isVisible)
	{
		DropItem->SetItemInteractCanvasVisibility(ESlateVisibility::Visible);
	}
	else
	{
		DropItem->SetItemInteractCanvasVisibility(ESlateVisibility::Hidden);
	}
}

