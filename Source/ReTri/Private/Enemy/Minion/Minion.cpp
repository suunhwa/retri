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
	// if (PlayerTarget)
	// {
	// 	CheckDistanceAndMove(DeltaTime);
	// }
}

void AMinion::CheckDistanceAndMove(float DeltaTime)
{
	// float Distance = GetDistanceTo(PlayerTarget);
	//
 //    
	// if (Distance > 300.0f) // 추격 범위
	// {
	// 	// 플레이어 향해 회전
	// 	RotateToTarget(DeltaTime, 5.0f);
	//
	// 	// 이동
	// 	if (AAIController* AIC = Cast<AAIController>(GetController()))
	// 	{
	// 		AIC->MoveToActor(PlayerTarget, 120.0f);
	// 	}
	// }
	// else // 공격 범위
	// {
	// 	if (!bIsAttacking && MyStatInfo && MyStatInfo->MinionAttackMontage)
	// 	{
	// 		bIsAttacking = true;
	// 		PlayAnimMontage(MyStatInfo->MinionAttackMontage);
 //            
	// 		// 몽타주 종료 후 bIsAttacking을 false로 돌리는 로직 필요 (애님노티파이)
	// 	}
	// }
}

float AMinion::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	// if (!IsValid(this)) return 0.0f;
	//
	// float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//
	// if (FSM)
	// {
	// 	if (CurrentHP - DamageAmount <= 0.0f)
	// 		FSM->_State = EMinionState::Die;
	// 	else
	// 		FSM->_State = EMinionState::Damage;
	// }
	// else 
	// {
	// 	CurrentHP -= DamageAmount;
	// }
	//
	// return ActualDamage;
	//
	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (ActualDamage > 0.0f && FSM != nullptr)
	{
		FlashRed();
		
		// === Damage UI ===
		if (auto* GI = Cast<UReTriGameInstance>(GetGameInstance()))
		{
			if (GI->FloatingUIActorClass)
			{
				AFloatingUIActor* DamageText = GetWorld()->SpawnActor<AFloatingUIActor>(
					GI->FloatingUIActorClass, 
					GetActorLocation(),
					FRotator::ZeroRotator
				);
				
				FString DmgString = FString::Printf(TEXT("%d"), FMath::RoundToInt(ActualDamage));
				DamageText->ShowFloatingUI(FText::FromString(DmgString), FLinearColor(1.0f, 0.612f, 0.057, 1.0f));
			}
		}
		
		if (CurrentHP <= 0.0f)
		{
			FSM->_State = EMinionState::Die;
			
		}
		else
		{
			FSM->_State = EMinionState::Attack;	// 나중에 Damage로 바꾸기
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

		//UE_LOG(LogTemp, Warning, TEXT("플레이어에게 %f 데미지를 입혔다!"), 1.0f);
	}
}

void AMinion::DoRagdoll()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	
	// Spawn Goods
	auto EXP = GetWorld()->SpawnActor<AGoodsExp>(GoodsEXPClass, GetActorLocation(), GetActorRotation());
	EXP->Amount = AmountExp;
}

void AMinion::FlashRed()
{
	if (GetMesh())
	{
		GetMesh()->SetVectorParameterValueOnMaterials(TEXT("OverlayColor"), FVector(10.0f, 0.0f, 0.0f));
		
		GetWorldTimerManager().SetTimer(FlashTimer, [this]()
		{
			if (GetMesh())
			{
				GetMesh()->SetVectorParameterValueOnMaterials(TEXT("OverlayColor"), FVector(0.0f, 0.0f, 0.0f));
			}
		}, 0.1f, false);
	}
}


void AMinion::PlayMontage(UAnimMontage* MontageToPlay)
{
	PlayAnimMontage(MontageToPlay);
}




