

#include "Merchant/Merchant.h"

#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemBase.h"
#include "Level/Data/InteractableData.h"
#include "Level/UI/InteractableInfoUI.h"
#include "Level/UI/InteractableUI.h"
#include "MapSubSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Level/Actors/FloatingUIActor.h"
#include "Merchant/Data/MerchantData.h"
#include "Merchant/UI/ShopBGUI.h"
#include "Merchant/UI/ShopSlotUI.h"
#include "Player/Abilities/AbilityBase.h"
#include "Player/PlayerCharacter.h"

#include "ReTri/ReTri.h"


AMerchant::AMerchant()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MerchantCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MerchantCollision"));
	MerchantCollision->SetupAttachment(RootComponent);
	MerchantCollision->SetSphereRadius(200.f);
	MerchantCollision->SetCollisionProfileName(TEXT("Interaction"));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (TempMesh.Succeeded()) GetMesh()->SetSkeletalMesh(TempMesh.Object, true);
	
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(RootComponent);
	InteractUI->SetWidgetSpace(EWidgetSpace::World); //! EWidgetSpace::World
	InteractUI->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	ConstructorHelpers::FClassFinder<UInteractableUI> TempInteractUI(TEXT("/Game/LevelInteraction/01_UI/WBP_InteractableUI.WBP_InteractableUI_C"));
	if (TempInteractUI.Succeeded()) InteractUI->SetWidgetClass(TempInteractUI.Class);
	InteractUI->SetVisibility(false);
}

void AMerchant::BeginPlay()
{
	Super::BeginPlay();
	
	CurHp = MaxHp;
	
	MerchantCollision->OnComponentBeginOverlap.AddDynamic(this, &AMerchant::MerchantBeginOverlap);
	MerchantCollision->OnComponentEndOverlap.AddDynamic(this, &AMerchant::MerchantEndOverlap);
	
	if (InteractUI)
	{
		UInteractableUI* InteractableUI = Cast<UInteractableUI>(InteractUI->GetUserWidgetObject());
		if (InteractableUI) 
			InteractableUI->InteractableName->SetText(FText::FromString(TEXT("계월 <대가를 징수하는 자>")));
	}
	
	if (MerchantDialogueDataTable)
	{
		MerchantDialogue.Empty();
		MerchantDialogueDataTable->GetAllRows<FMerchantDialogueRow>(TEXT("Curse::Interact"), MerchantDialogue);
	}
}

void AMerchant::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (MerchantUIInstance && MerchantUIInstance->IsInViewport())
	{
		MerchantUIInstance->RemoveFromParent();
	}
	
	Super::EndPlay(EndPlayReason);
}

void AMerchant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsInteractable && GetWorld())
	{
		APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		if (CamManager)
		{
			FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(InteractUI->GetComponentLocation(), CamManager->GetCameraLocation());
			InteractUI->SetWorldRotation(LookAtRot);
		}
	}
}

void AMerchant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void AMerchant::Interact_Implementation()
{
	IInteractableInterface::Interact_Implementation();
	
	ShowMerchantUI();
	
	if (auto GI = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		if (auto MapSub = GI->GetSubsystem<UMapSubSystem>())
		{
			FShopItemSkillData* SkillDatas = MapSub->MerchantItemDatas.Find(MapSub->CurMapIndex);
			if (SkillDatas)
			{
				for (int32 i = 0; i < SkillDatas->ItemSkillDatas.Num(); i++)
				{
					FPlayerSkillData Skill = SkillDatas->ItemSkillDatas[i];
					if (MerchantUIInstance)
					{
						UShopSlotUI* SlotUI = MerchantUIInstance->AddButton(i, Skill.Icon, Skill.UpgradeCostGold);
						if (SlotUI)
							SlotUI->OnSlotClicked.AddDynamic(this, &AMerchant::OnClickedMerchantSlotUI);
					}
				}
			}
		}
	}
}

void AMerchant::MerchantBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(APlayerCharacter::StaticClass())) return;
	
	bIsInteractable = true;
	InteractUI->SetVisibility(true);
	UGameplayStatics::PlaySound2D(GetWorld(), OverlapSound);
}

void AMerchant::MerchantEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsInteractable = false;
	InteractUI->SetVisibility(false);
	if (MerchantUIInstance)
		HideMerchantUI();
}

float AMerchant::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (auto* GI = Cast<UReTriGameInstance>(GetGameInstance()))
	{
		if (GI->FloatingUIActorClass)
		{
			AFloatingUIActor* DamageText = GetWorld()->SpawnActor<AFloatingUIActor>(
				GI->FloatingUIActorClass, 
				GetActorLocation(),
				FRotator::ZeroRotator
				);
			
			int32 RanNum = FMath::RandRange(0, MerchantDialogue.Num()-1);
			
			DamageText->ShowFloatingLongUI(FText::FromString(MerchantDialogue[RanNum]->Dialogue), FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	
	return Damage;
}

void AMerchant::ShowMerchantUI()
{
	if (!MerchantUIClass) return;
	
	if (!MerchantUIInstance)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		MerchantUIInstance = CreateWidget<UShopBGUI>(PC, MerchantUIClass);
		if (MerchantUIInstance)
			MerchantUIInstance->ClearFlags(RF_Transactional);
	}

	if (!MerchantUIInstance) return;

	MerchantUIInstance->ClearButtons();
	if (!MerchantUIInstance->IsInViewport())
	{
		MerchantUIInstance->AddToViewport();
		UGameplayStatics::PlaySound2D(GetWorld(), InteractSound);
	}
}

void AMerchant::HideMerchantUI()
{
	if (!MerchantUIInstance) return;
	if (MerchantUIInstance->IsInViewport())
	{
		MerchantUIInstance->RemoveFromParent();
	}
}

void AMerchant::OnClickedMerchantSlotUI(int32 SlotNum)
{	
	if (!ItemClass) return;
	
	HideMerchantUI();
	
	if (auto* GI = Cast<UReTriGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (auto* MapSub = GI->GetSubsystem<UMapSubSystem>())
		{
			// UE_LOG(LogTemp, Warning, TEXT("AMerchant: 현재 층 (CurMapIndex: %d)"), MapSub->CurMapIndex);
			// UE_LOG(LogTemp, Warning, TEXT("AMerchant:  (MerchantItemDatas 개수: %d)"), MapSub->MerchantItemDatas.Num());
			FShopItemSkillData* SkillDatas = MapSub->MerchantItemDatas.Find(MapSub->CurMapIndex);
			
			FString FloatingText = FString::Printf(TEXT("골드 부족!"));
			FLinearColor FloatingColor = FLinearColor::Red;
	
			if (!GI->StatComp->SpendGold(SkillDatas->ItemSkillDatas[SlotNum].UpgradeCostGold))
			{
				if (GI->FloatingUIActorClass)
				{
					if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
					{
						AFloatingUIActor* FloatingUIActor = GetWorld()->SpawnActor<AFloatingUIActor>(
							GI->FloatingUIActorClass, Player->GetActorLocation(), FRotator::ZeroRotator);
						FloatingUIActor->ShowFloatingUI(FText::FromString(FloatingText), FloatingColor);

						UGameplayStatics::PlaySound2D(GetWorld(), FailedSound);
					}
				}
		
				return;
			}
			
			FVector Loc = GetActorLocation() + (GetActorRightVector() * 300.f);
			auto* Item = GetWorld()->SpawnActor<AItemBase>(ItemClass, Loc, FRotator::ZeroRotator);
			
			if (Item)
			{
				const FPlayerSkillData& SkillData = SkillDatas->ItemSkillDatas[SlotNum];
				Item->DataInit(SkillData);

				if (TSubclassOf<UAbilityBase>* FoundClass = SkillIDToClassMap.Find(SkillData.SkillID))
				{
					Item->AbilityClass = *FoundClass;
				}
				
				MapSub->RemoveMerchantItemList(MapSub->CurMapIndex, SlotNum);
			}
		}
	}
}

