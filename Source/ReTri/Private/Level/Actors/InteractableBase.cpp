// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/Actors/InteractableBase.h"

#include "Level/Actors/NZW_TestPlayer.h"
#include "Level/Data/InteractableData.h"
#include "Level/UI/InteractableInfoUI.h"
#include "Level/UI/InteractableUI.h"
#include "Level/UI/SelectUI.h"
#include "MapSubSystem.h"

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
	
	InteractUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractUI"));
	InteractUI->SetupAttachment(CapsuleComp);
	InteractUI->SetWidgetSpace(EWidgetSpace::Screen); //! EWidgetSpace::World
	InteractUI->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	InteractUI->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
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
	// GC에서 해결 되지 않고 플레이를 끈 경우 
	if (SelectUIInstance && SelectUIInstance->IsInViewport())
	{
		SelectUIInstance->RemoveFromParent();
	}
	
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
	
	//UIFocus();
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

void AInteractableBase::ShowSelectUI()
{
	if (!SelectUIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Select UI Class 할당 안됨"));
		return;
	}
	
	// 최소한 한 번만 생성, 이후에는 인스턴스 재사용
	if (!SelectUIInstance)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		SelectUIInstance = CreateWidget<USelectUI>(PC, SelectUIClass);
		if (SelectUIInstance)
		{
			// PIE Undo Buffer Leaks 방지
			// 위 코드는 위젯을 생성하자마자 
			// "이 위젯은 실행 취소 기록에 남기지 마! 넌 그냥 게임 플레이 도중에 쓰고 버릴 일회용이야!" 
			// 라고 플래그(RF_Transactional)를 강제로 지워버리는 역할
			SelectUIInstance->ClearFlags(RF_Transactional);
		}
	}

	if (!SelectUIInstance) return;

	// 이전 버튼들 정리 
	SelectUIInstance->ClearButtons();
	if (!SelectUIInstance->IsInViewport())
	{
		SelectUIInstance->AddToViewport();
	}
}

void AInteractableBase::HideSelectUI()
{
	if (SelectUIInstance->IsInViewport())
	{
		SelectUIInstance->RemoveFromParent();
	}
}
