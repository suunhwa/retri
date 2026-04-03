// Fill out your copyright notice in the Description page of Project Settings.


#include "Merchant/Merchant.h"

#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Level/Data/InteractableData.h"
#include "Level/UI/InteractableInfoUI.h"
#include "Level/UI/InteractableUI.h"
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
	
	//todo 여기 ConstructorHelpers 이부분 고쳐야함
	//! MerchantUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("MerchantUI"));
	//! MerchantUI->SetupAttachment(RootComponent);
	//! ConstructorHelpers::FClassFinder<UInteractableUI> TempMerchantUI(TEXT("/Game/LevelInteraction/01_UI/WBP_InteractableUI.WBP_InteractableUI_C"));
	//! if (TempMerchantUI.Succeeded()) InteractUI->SetWidgetClass(TempMerchantUI.Class);
	//! InteractUI->SetVisibility(false);
	
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
	
	// todo: 상점 UI Random Slot 4~6개 띄우기
	// todo: 상점 UI 띄우기
	UE_LOG(jiwon, Warning, TEXT("상점이다!"));
}

void AMerchant::MerchantBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AMerchant::ShowSelectUI()
{
}

void AMerchant::HideSelectUI()
{
}

