// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Minion/Minion.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/Minion/MyMinionFSM.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "ReTriGameInstance.h"
#include "Level/Actors/FloatingUIActor.h"
#include "Level/Actors/GoodsExp.h"
#include "Player/PlayerCharacter.h"


AMinion::AMinion()
{
	FSM = CreateDefaultSubobject<UMyMinionFSM>(TEXT("FSM"));
	
	PaperComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperComp"));
	PaperComp->SetupAttachment(RootComponent);	
	ConstructorHelpers::FObjectFinder<UPaperSprite> TempPaper(TEXT("/Script/Paper2D.PaperSprite'/Game/LevelInteraction/03_Assets/Icon/_Enemy_Sprite._Enemy_Sprite'"));
	if (TempPaper.Succeeded()) PaperComp->SetSprite(TempPaper.Object);
	PaperComp->SetRelativeLocation(FVector(0.f, 0.f, 2750.f));
	PaperComp->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	PaperComp->SetRelativeScale3D(FVector(8.f, 1.f, 8.f));
	PaperComp->SetCastShadow(false);
}

void AMinion::BeginPlay()
{
	Super::BeginPlay();
	
	FSM  = Cast<UMyMinionFSM>(GetComponentByClass(UMyMinionFSM::StaticClass()));
}

void AMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


float AMinion::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (ActualDamage > 0.0f && FSM != nullptr)
	{
		PaperComp->SetVisibility(false);
		// === 피격 사운드 랜덤 재생 ===
		if (HitSounds.Num() > 0)
		{
			int32 RandIdx = FMath::RandRange(0, HitSounds.Num() - 1);
			if (HitSounds[RandIdx])
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSounds[RandIdx], GetActorLocation());
			}
		}
		
		// === Damage UI ===
		if (auto* GI = Cast<UReTriGameInstance>(GetGameInstance()))
		{
			// === GamePlay Save ===
			GI->PlayerPlayData.SetGiveDamage(ActualDamage);
			
			if (GI->FloatingUIActorClass)
			{
				AFloatingUIActor* DamageText = GetWorld()->SpawnActor<AFloatingUIActor>(
					GI->FloatingUIActorClass, 
					GetActorLocation(),
					FRotator::ZeroRotator
				);
				
				FString DmgString = FString::Printf(TEXT("%d"), FMath::RoundToInt(ActualDamage));
				DamageText->ShowScaleUI(FText::FromString(DmgString), FLinearColor(1.0f, 0.612f, 0.057, 1.0f));
			}
		}
		
		if (CurrentHP <= 0.0f)
		{
			FSM->_State = EMinionState::Die;
			
		}
		else
		{
			FSM->_State = EMinionState::Attack;
		}
	}
	return ActualDamage;
}

void AMinion::OnAttackOverlap(AActor* OtherActor)
{
	if (!OtherActor->IsA(APlayerCharacter::StaticClass())) return;

	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player)
	{
		UGameplayStatics::ApplyDamage
		(
			OtherActor,           // 맞는 대상
			1.0f,   
			GetController(),      // 보스의 컨트롤러
			this,                 // 미니언 자신
			nullptr               // 특별한 타입 없으면 null
		);
		
		bHasHitTarget = true;
	}
}

void AMinion::DoRagdoll()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	
	// === Spawn EXP Goods ===
	if (GoodsEXPClass)
	{
		FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
		AGoodsExp* EXP = GetWorld()->SpawnActorDeferred<AGoodsExp>(
			GoodsEXPClass, 
			SpawnTransform, 
			nullptr, 
			nullptr, 
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		
		if (EXP)
		{
			EXP->Amount = AmountExp;
			UGameplayStatics::FinishSpawningActor(EXP, SpawnTransform);
		}
	}
}

void AMinion::PlayMontage(UAnimMontage* MontageToPlay)
{
	PlayAnimMontage(MontageToPlay);
}




