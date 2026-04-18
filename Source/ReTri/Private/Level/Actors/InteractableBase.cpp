

#include "Level/Actors/InteractableBase.h"

#include "Level/Data/InteractableData.h"
#include "Level/UI/InteractableInfoUI.h"
#include "Level/UI/InteractableUI.h"
#include "MapSubSystem.h"
#include "NiagaraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


AInteractableBase::AInteractableBase()
{
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
	
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(CapsuleComp);
	
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(CapsuleComp);
	InteractUI->SetWidgetSpace(EWidgetSpace::World); 
	InteractUI->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	ConstructorHelpers::FClassFinder<UInteractableUI> TempUI(TEXT("/Game/LevelInteraction/01_UI/WBP_InteractableUI.WBP_InteractableUI_C"));
	if (TempUI.Succeeded()) InteractUI->SetWidgetClass(TempUI.Class);
	InteractUI->SetVisibility(false);
		
	InteractInfoUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractInfoUI"));
	InteractInfoUI->SetupAttachment(CapsuleComp);
	InteractInfoUI->SetWidgetSpace(EWidgetSpace::Screen);
	ConstructorHelpers::FClassFinder<UInteractableInfoUI> TempInfoUI(TEXT("/Game/LevelInteraction/01_UI/WBP_InteractableInfoUI.WBP_InteractableInfoUI_C"));
	if (TempInfoUI.Succeeded()) InteractInfoUI->SetWidgetClass(TempInfoUI.Class);
	InteractInfoUI->SetVisibility(false); 

	ConstructorHelpers::FObjectFinder<USoundBase> TempOverlap(TEXT("/Game/LevelInteraction/03_Assets/Sound/OverlapSound.OverlapSound"));
	if (TempOverlap.Succeeded()) OverlapSound = TempOverlap.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> TempInteract(TEXT("/Game/LevelInteraction/03_Assets/Sound/InteractSound.InteractSound"));
	if (TempInteract.Succeeded()) InteractSound = TempInteract.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> TempSelect(TEXT("/Game/LevelInteraction/03_Assets/Sound/SelectSound.SelectSound"));
	if (TempSelect.Succeeded()) SelectSound = TempSelect.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> TempFaild(TEXT("/Game/LevelInteraction/03_Assets/Sound/NotEnoughMoney.NotEnoughMoney"));
	if (TempFaild.Succeeded()) FailedSound = TempFaild.Object;
}

void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurDescriptionTime = MaxDescriptionTime;
}

void AInteractableBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

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
	InteractableType = RowData.InteractableType; 
	InteractName = RowData.InteractName;
	Description = RowData.Description;
	UStaticMesh* LoadMesh = RowData.InteractableMesh.LoadSynchronous();
	if (LoadMesh)
		MeshComp->SetStaticMesh(LoadMesh);
	
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
	auto* IUI = Cast<UInteractableUI>(InteractUI->GetUserWidgetObject());
	if (IUI) IUI->PlayBigAnimation();
	UGameplayStatics::PlaySound2D(GetWorld(), OverlapSound);
}

void AInteractableBase::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	MyPlayer = nullptr;
	bIsInteractable = false;
	InteractUI->SetVisibility(false);
	auto* IUI = Cast<UInteractableUI>(InteractUI->GetUserWidgetObject());	
	if (IUI) IUI->ReverseBigAnimation();
}

void AInteractableBase::UIFocus()
{
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

void AInteractableBase::UIHold()
{
	if (bIsHovering)
		InteractInfoUI->SetVisibility(true);
	else
		InteractInfoUI->SetVisibility(false);
}
