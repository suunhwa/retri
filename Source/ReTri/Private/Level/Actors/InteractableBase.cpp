// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractableBase.h"

#include "Level/Data/InteractableData.h"
#include "Level/UI/InteractableInfoUI.h"
#include "Level/UI/InteractableUI.h"
#include "Level/UI/SelectUI.h"
#include "MapSubSystem.h"
#include "Level/Actors/InteractChaos.h"

#include "Components/CapsuleComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCollisionProfileName(TEXT("Interaction"));
	CapsuleComp->SetCapsuleHalfHeight(200.f);
	CapsuleComp->SetCapsuleRadius(150.f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);	
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComp->GetScaledCapsuleHalfHeight()));
	MeshComp->SetRelativeScale3D(FVector(1.2f, 1.2f, 1.2f));
	
	PaperComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperComp"));
	PaperComp->SetupAttachment(CapsuleComp);	
	ConstructorHelpers::FObjectFinder<UPaperSprite> TempPaper(TEXT("/Script/Paper2D.PaperSprite'/Game/LevelInteraction/03_Assets/Icon/_Interactable_Sprite._Interactable_Sprite'"));
	if (TempPaper.Succeeded()) PaperComp->SetSprite(TempPaper.Object);
	PaperComp->SetRelativeLocation(FVector(0.f, 0.f, 2750.f));
	PaperComp->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	PaperComp->SetRelativeScale3D(FVector(8.f, 1.f, 8.f));
	PaperComp->SetCastShadow(false);
	
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(CapsuleComp);
	InteractUI->SetWidgetSpace(EWidgetSpace::World); //! EWidgetSpace::World
	InteractUI->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	//InteractUI->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	ConstructorHelpers::FClassFinder<UInteractableUI> TempUI(TEXT("/Game/LevelInteraction/01_UI/WBP_InteractableUI.WBP_InteractableUI_C"));
	if (TempUI.Succeeded()) InteractUI->SetWidgetClass(TempUI.Class);
	InteractUI->SetVisibility(false);
		
	InteractInfoUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractInfoUI"));
	InteractInfoUI->SetupAttachment(CapsuleComp);
	InteractInfoUI->SetWidgetSpace(EWidgetSpace::Screen);
	ConstructorHelpers::FClassFinder<UInteractableInfoUI> TempInfoUI(TEXT("/Game/LevelInteraction/01_UI/WBP_InteractableInfoUI.WBP_InteractableInfoUI_C"));
	if (TempInfoUI.Succeeded()) InteractInfoUI->SetWidgetClass(TempInfoUI.Class);
	InteractInfoUI->SetVisibility(false); 
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurDescriptionTime = MaxDescriptionTime;
}

void AInteractableBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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

void AInteractableBase::DataInit(FName InRowName, FInteractableData RowData)
{
	MyRowName = InRowName;
	//InteractableData = RowData;
	InteractableType = RowData.InteractableType; 
	InteractName = RowData.InteractName;
	Description = RowData.Description;
	UStaticMesh* LoadMesh = RowData.InteractableMesh.LoadSynchronous();
	if (LoadMesh)
		MeshComp->SetStaticMesh(LoadMesh);
	else
		UE_LOG(LogTemp, Warning, TEXT("Load 매시 로드 안됨 왜?"));
	
	UInteractableUI* InteractableUI = Cast<UInteractableUI>(InteractUI->GetUserWidgetObject());
	if (InteractableUI) 
		InteractableUI->InteractableName->SetText(FText::FromString(InteractName));
	UInteractableInfoUI* InteractableInfoUI = Cast<UInteractableInfoUI>(InteractInfoUI->GetUserWidgetObject());
	if (InteractableInfoUI)
		InteractableInfoUI->InfoText->SetText(FText::FromString(Description));
	
	FVector Pos = GetActorLocation();
	SetActorLocation(FVector(Pos.X, Pos.Y, Pos.Z+CapsuleComp->GetScaledCapsuleHalfHeight()));
	
	if (bIsUsed)
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AInteractableBase::SetIsUsed(bool IsUsed)
{
	bIsUsed = IsUsed;
	if (bIsUsed)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UMapSubSystem* MapSub = GI->GetSubsystem<UMapSubSystem>())
			{
				MapSub->SetInteractableUsed(MyRowName);
			}
		}
	}
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
}

void AInteractableBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	MyPlayer = Cast<APawn>(OtherActor);
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
