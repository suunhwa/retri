

#include "Item/ItemBase.h"
#include "Item/UI/DropItemUI.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Player/Components/AbilityComponent.h"
#include "Player/Components/StatComponent.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(150.f);
	SphereComp->SetCollisionObjectType(ECC_GameTraceChannel6);
	
	ItemUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	ItemUI->SetupAttachment(SphereComp);
	ItemUI->SetWidgetSpace(EWidgetSpace::World); 
	ConstructorHelpers::FClassFinder<UDropItemUI> TempUI(TEXT("/Game/Item/UI/WBP_DropItemUI.WBP_DropItemUI_C"));
	if (TempUI.Succeeded()) ItemUI->SetWidgetClass(TempUI.Class);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (CamManager)
	{
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
	
	if (!AbilityClass) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player) return;

	UAbilityComponent* AbilityComp = Player->GetAbilityComponent();
	if (!AbilityComp) return;

	if (AbilityComp->EquipAcquiredSkill(AbilityClass))
	{
		if (EquipSuccessSound)
			UGameplayStatics::PlaySound2D(GetWorld(), EquipSuccessSound);

		Destroy();
	}
	else
	{
		if (EquipFullSound)
			UGameplayStatics::PlaySound2D(GetWorld(), EquipFullSound);
	}
}

void AItemBase::Hold_Implementation(AActor* PlayerActor)
{
	ISkillItemInterface::Hold_Implementation(PlayerActor);
	
	if (!PlayerActor->IsA(APlayerCharacter::StaticClass())) return;
	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerActor);
	
	UStatComponent* StatComp = Player->GetStatComponent();
	if (!StatComp) return;
	
	int32 RewardAmount = CurSkillData.UpgradeCostDreamDust;
	if (RewardAmount <= 0) return;

	StatComp->ApplyStatModifier(EStatTypes::DreamDust, static_cast<float>(RewardAmount));
	
	Destroy();
}

void AItemBase::DataInit(FPlayerSkillData SkillData)
{
	CurSkillData = SkillData;
	
	if (!ItemUI) return;
	
	UDropItemUI* DropItem = Cast<UDropItemUI>(ItemUI->GetUserWidgetObject());
	if (!DropItem) return;
	DropItem->SetItemIcon(SkillData.Icon);
	DropItem->SetItemName(SkillData.SkillNameKR);
	DropItem->SetDecomposeBar(0.0f);
}

void AItemBase::SetInteractionUIVisibility(bool isVisible)
{
	UDropItemUI* DropItem = Cast<UDropItemUI>(ItemUI->GetUserWidgetObject());
	if (!DropItem) return;
	
	if (isVisible)
		DropItem->SetItemInteractCanvasVisibility(ESlateVisibility::Visible);
	else
		DropItem->SetItemInteractCanvasVisibility(ESlateVisibility::Hidden);
}

