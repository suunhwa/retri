

#include "Level/Actors/LootBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ALootBase::ALootBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BoxComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetCapsuleHalfHeight(200.f);
	CapsuleComp->SetCapsuleRadius(150.f);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);
	
	ConstructorHelpers::FObjectFinder<USoundBase> TempDam(TEXT("/Game/LevelInteraction/03_Assets/Sound/sfxGemTap0.sfxGemTap0"));
	if (TempDam.Succeeded()) DamageSound = TempDam.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> TempBre1(TEXT("/Game/LevelInteraction/03_Assets/Sound/Break1.Break1"));
	if (TempBre1.Succeeded()) BreakSound1 = TempBre1.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> TempBre2(TEXT("/Game/LevelInteraction/03_Assets/Sound/Break2.Break2"));
	if (TempBre2.Succeeded()) BreakSound2 = TempBre2.Object;
}

void ALootBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALootBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ALootBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) return;
	if (bIsBroken == true) return;
	
	OtherActor->Destroy();
	
	HP -= 1;
	
	if (HP <= 0)
	{
		bIsBroken = true;
		Break();
		return;
	}
	
	UGameplayStatics::PlaySound2D(GetWorld(), DamageSound);
}

void ALootBase::Break()
{
	Destroy();
}

