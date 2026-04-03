// Fill out your copyright notice in the Description page of Project Settings.


#include "Merchant/Merchant.h"

#include "MapSubSystem.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Level/Data/InteractableData.h"
#include "Level/UI/InteractableInfoUI.h"
#include "Level/UI/InteractableUI.h"
#include "Merchant/UI/ShopBGUI.h"
#include "Merchant/UI/ShopSlotUI.h"
#include "Player/PlayerCharacter.h"
#include "ReTri/ReTri.h"


// Sets default values
AMerchant::AMerchant()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MerchantCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MerchantCollision"));
	MerchantCollision->SetupAttachment(RootComponent);
	MerchantCollision->SetSphereRadius(250.f);
	MerchantCollision->SetCollisionProfileName(TEXT("Interaction"));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));
	if (TempMesh.Succeeded()) GetMesh()->SetSkeletalMesh(TempMesh.Object, true);
	//  아래 경로는 붉은색 Skeletal Mesh. 뚜까 맞으면 이걸로 바꾸는것,,? 미리 받아놔야할듯!
	// /Script/Engine.SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Skins/Tier_1/Sevarog_Red/Meshes/SevarogBloodred.SevarogBloodred'
	
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(RootComponent);
	InteractUI->SetWidgetSpace(EWidgetSpace::Screen); //! EWidgetSpace::World
	InteractUI->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	InteractUI->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	ConstructorHelpers::FClassFinder<UInteractableUI> TempInteractUI(TEXT("/Game/LevelInteraction/01_UI/WBP_InteractableUI.WBP_InteractableUI_C"));
	if (TempInteractUI.Succeeded()) InteractUI->SetWidgetClass(TempInteractUI.Class);
	InteractUI->SetVisibility(false);
	
	// MerchantUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("MerchantUI"));
	// MerchantUI->SetupAttachment(RootComponent);
	// MerchantUI->SetWidgetSpace(EWidgetSpace::Screen);
	// ConstructorHelpers::FClassFinder<UShopBGUI> TempMerchantUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Merchant/01_UI/WBP_ShopBG.WBP_ShopBG_C'"));
	// if (TempMerchantUI.Succeeded()) MerchantUI->SetWidgetClass(TempMerchantUI.Class);
	// MerchantUI->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMerchant::BeginPlay()
{
	Super::BeginPlay();
	
	// 상점 Collider Overlap 
	MerchantCollision->OnComponentBeginOverlap.AddDynamic(this, &AMerchant::MerchantBeginOverlap);
	MerchantCollision->OnComponentEndOverlap.AddDynamic(this, &AMerchant::MerchantEndOverlap);
	
	if (InteractUI)
	{
		UInteractableUI* InteractableUI = Cast<UInteractableUI>(InteractUI->GetUserWidgetObject());
		if (InteractableUI) 
			InteractableUI->InteractableName->SetText(FText::FromString(TEXT("[상인] 조너스")));
	}
	
}

void AMerchant::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// GC에서 해결 되지 않고 플레이를 강제로 껐을 때 PIE World Leak 방지
	if (MerchantUIInstance && MerchantUIInstance->IsInViewport())
	{
		MerchantUIInstance->RemoveFromParent();
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AMerchant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMerchant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void AMerchant::Interact_Implementation()
{
	IInteractableInterface::Interact_Implementation();
	
	UE_LOG(jiwon, Warning, TEXT("상점 UI 띄우기"));
	
	// todo: 상점 UI Random Slot 4~6개 띄우기 -> Game Instance
	// todo: 상점 UI 띄우기
	ShowMerchantUI();
	
	if (auto GI = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		if (auto MapSub = GI->GetSubsystem<UMapSubSystem>())
		{
			
			UE_LOG(LogTemp, Warning, TEXT("AMerchant: 현재 층 (CurMapIndex: %d)"), MapSub->CurMapIndex);
			UE_LOG(LogTemp, Warning, TEXT("AMerchant:  (MerchantItemDatas 개수: %d)"), MapSub->MerchantItemDatas.Num());
			FShopItemSkillData* SkillDatas = MapSub->MerchantItemDatas.Find(MapSub->CurMapIndex);
			// 데이터가 정상적으로 찾아졌는지 체크 (nullptr 방어)
			if (SkillDatas)
			{
				for (FPlayerSkillData Skill : SkillDatas->ItemSkillDatas)
				{
					if (MerchantUIInstance)
					{
						UShopSlotUI* SlotUI = MerchantUIInstance->AddButton(Skill.Icon, Skill.UpgradeCostGold);
						// 버튼 생성 성공 여부 체크 
						if (SlotUI)
						{
							SlotUI->OnSlotClicked.AddDynamic(this, &AMerchant::OnClickedMerchantSlotUI);
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AMerchant: 상점 데이터를 찾을 수 없습니다. (CurMapIndex: %d)"), MapSub->CurMapIndex);
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
}

void AMerchant::MerchantEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsInteractable = false;
	InteractUI->SetVisibility(false);
}

void AMerchant::DataInit(FName InRowName, FInteractableData RowData)
{
	
}

void AMerchant::ShowMerchantUI()
{
	if (!MerchantUIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("MerchantUIClass 할당 안됨"));
		return;
	}
	
	// 최소한 한 번만 생성, 이후에는 인스턴스 재사용
	if (!MerchantUIInstance)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		MerchantUIInstance = CreateWidget<UShopBGUI>(PC, MerchantUIClass);
		if (MerchantUIInstance)
		{
			// PIE Undo Buffer Leaks 방지
			// 위 코드는 위젯을 생성하자마자 
			// "이 위젯은 실행 취소 기록에 남기지 마! 넌 그냥 게임 플레이 도중에 쓰고 버릴 일회용이야!" 
			// 라고 플래그(RF_Transactional)를 강제로 지워버리는 역할
			MerchantUIInstance->ClearFlags(RF_Transactional);
		}
	}

	if (!MerchantUIInstance) return;

	// 이전 버튼들 정리 
	MerchantUIInstance->ClearButtons();
	if (!MerchantUIInstance->IsInViewport())
	{
		MerchantUIInstance->AddToViewport();
	}
}

void AMerchant::HideMerchantUI()
{
	if (MerchantUIInstance->IsInViewport())
	{
		MerchantUIInstance->RemoveFromParent();
	}
}

void AMerchant::OnClickedMerchantSlotUI()
{	
	SCREENLOG("플레이어의 아이템 슬롯이 비어있으면 들어가고 꽉 차 있으면 바닥에 버려짐..?");
}

