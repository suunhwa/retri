// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractableBase.h"
#include "Level/Actors/NZW_TestPlayer.h"
#include "Level/Data/InteractableData.h"
#include "Level/UI/InteractableInfoUI.h"
#include "Level/UI/InteractableUI.h"

#include "Components/CapsuleComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BoxComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCollisionProfileName(TEXT("Interaction"));
	CapsuleComp->SetCapsuleHalfHeight(200.f);
	CapsuleComp->SetCapsuleRadius(150.f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	MeshComp->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
	
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(CapsuleComp);
	InteractUI->SetWidgetSpace(EWidgetSpace::World);
	InteractUI->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	InteractUI->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	ConstructorHelpers::FClassFinder<UInteractableUI> TempUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LevelInteraction/01_UI/WBP_InteractableUI.WBP_InteractableUI_C'"));
	if (TempUI.Succeeded()) InteractUI->SetWidgetClass(TempUI.Class);
	InteractUI->SetVisibility(false);
		
	InteractInfoUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractInfoUI"));
	InteractInfoUI->SetupAttachment(CapsuleComp);
	InteractInfoUI->SetWidgetSpace(EWidgetSpace::Screen);
	ConstructorHelpers::FClassFinder<UInteractableInfoUI> TempInfoUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LevelInteraction/01_UI/WBP_InteractableInfoUI.WBP_InteractableInfoUI_C'"));
	if (TempInfoUI.Succeeded()) InteractInfoUI->SetWidgetClass(TempInfoUI.Class);
	InteractInfoUI->SetVisibility(false); 
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurDescriptionTime = MaxDescriptionTime;
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurDescriptionTime < MaxDescriptionTime)
	{
		bIsHovering = true;
		CurDescriptionTime += DeltaTime;
	}
	else
	{
		bIsHovering = false;
	}
	
	UIFocus();
	UIHold();
}

void AInteractableBase::DataInit(FInteractableData RowData)
{
	InteractableType = RowData.InteractableType; 
	InteractName = RowData.InteractName;
	Description = RowData.Description;
	//!Probability = RowData.Probability;
	
	UInteractableUI* InteractableUI = Cast<UInteractableUI>(InteractUI->GetUserWidgetObject());
	if (InteractableUI) 
		InteractableUI->InteractableName->SetText(FText::FromString(InteractName));
	UInteractableInfoUI* InteractableInfoUI = Cast<UInteractableInfoUI>(InteractInfoUI->GetUserWidgetObject());
	if (InteractableInfoUI)
		InteractableInfoUI->InfoText->SetText(FText::FromString(Description)); 
}

void AInteractableBase::Hover_Implementation()
{
	IInteractableInterface::Hover_Implementation();
	
	if (!bIsInteractable) return;
	
	//UE_LOG(LogTemp, Warning, TEXT("Hover 했을 때 ?"));
	CurDescriptionTime = 0;
}

void AInteractableBase::Interact_Implementation()
{
	IInteractableInterface::Interact_Implementation();
	
	// 여기에 Interact F 키 눌렀을 때 플레이어가 실행할 함수 내용 각각 적용
	UE_LOG(LogTemp, Warning, TEXT("Interactable Base Class"));
}

void AInteractableBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	MyPlayer = Cast<ANZW_TestPlayer>(OtherActor);
	if (!MyPlayer) return;
	
	bIsInteractable = true;
	InteractUI->SetVisibility(true);
}

void AInteractableBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	MyPlayer = nullptr;
	bIsInteractable = false;
	InteractUI->SetVisibility(false);
}

void AInteractableBase::UIFocus()
{
	if (bIsInteractable && GetWorld())
	{
		APlayerCameraManager* CamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		if (CamManager)
		{
			// UI 방향을 카메라를 바라볼 수 있도록
			FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(InteractUI->GetComponentLocation(), CamManager->GetCameraLocation());
			InteractUI->SetWorldRotation(LookAtRot);
		}
	}
}

void AInteractableBase::UIHold()
{
	if (bIsHovering)
		InteractInfoUI->SetVisibility(true);
	else
		InteractInfoUI->SetVisibility(false);
}
