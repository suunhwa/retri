// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DarkMoon/DarkMoon.h"

#include "AIController.h"
#include "MapSubSystem.h"
#include "ReTriGameInstance.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"


ADarkMoon::ADarkMoon()
{
	bIsBoss = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Skins/GlacialEmpress/Meshes/Aurora_GlacialEmpress.Aurora_GlacialEmpress'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -180.0f), FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	}
	GetCapsuleComponent()->InitCapsuleSize(80.0f, 196.0f);
	
	
	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	
	if (GetMesh())
	{
		SwordCollision->SetupAttachment(GetMesh(), TEXT("SwordSocket"));
		SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SwordCollision->SetRelativeLocation(FVector(0.000000,65.000000,0.000000));
		SwordCollision->SetBoxExtent(FVector(14.175652,58.968844,10.359240));
		SwordCollision->SetGenerateOverlapEvents(true);
	}
}

void ADarkMoon::BeginPlay()
{
	Super::BeginPlay();
	
	SwordCollision->AttachToComponent(GetMesh(), 
		FAttachmentTransformRules::KeepRelativeTransform, 
		TEXT("SwordSocket"));
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADarkMoon::OnOverlapBegin);
	SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &ADarkMoon::OnSwordOverlap);
	
	if (StatDataTable)
	{
		FEnemyDataTableRow* Data = StatDataTable->FindRow<FEnemyDataTableRow>(EnemyRowName, TEXT("EnemyBegin"));
		
		if (Data)
		{
			MaxHP = Data->MaxHP;
			CurrentHP = MaxHP;
			
			BossSkills = Data->BossSkillsID;
			
			//UE_LOG(LogTemp, Warning, TEXT("%s 등장 스킬 %d종류 장착"), *EnemyRowName.ToString(), BossSkills.Num());
		}
		else
		{
			// UE_LOG(LogTemp, Log, TEXT("[오류] %s행 못 찾음"), *EnemyRowName.ToString());
		}
	}
	
}

void ADarkMoon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::L))
	// {
	// 	StartBattleEvent();
	// }
	
	// 치트키
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
	{
		ReduceBossHP();
	}
	
}

void ADarkMoon::ReduceBossHP()	// 치트키
{
	CurrentHP -=(MaxHP * 0.2f);
	if (CurrentHP < 0) CurrentHP = 0;
	
	// UE_LOG(LogTemp, Warning, TEXT("==== [치트키] 현재 체력: %.1f / %.1f ===="), CurrentHP, MaxHP);
	
	UpdatePhase();
}

void ADarkMoon::UpdatePhase()
{
	float HPRatio = CurrentHP / MaxHP;
	if (HPRatio < 0.3f)
	{
		CurrentPhase = 3;
	}
	else if (HPRatio < 0.6f)
	{
		CurrentPhase = 2;
	}
	else
	{
		CurrentPhase = 1;
	}
}


// void ADarkMoon::SetSwordCollisionEnabled(bool bEnabled)
// {
// 	if (bEnabled)
// 		SwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
// 	else
// 		SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// }


void ADarkMoon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ADarkMoon::OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnAttackOverlap(OtherActor);
}

float ADarkMoon::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	// EnemyBase 로직 먼저 실행
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// 보스 전용, 아직 싸움 시작 전인데 맞았다면? Battle
	if (!bIsBattleStarted && ActualDamage > 0.0f)
	{
		bIsBattleStarted = true;
		
		StartBattleEvent(); 
	}
	
	if (CurrentHP <= 0.1f)
	{
		BossDead();
	}
	return ActualDamage;
}

void ADarkMoon::BossDead()
{
	if (bIsDead) return;
	bIsDead = true;
	
	PlayDeathEffect();
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
	
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	if (Anim)
	{
		Anim->Montage_Stop(0.2f);
	}
	
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->StopMovement();
		AIC->UnPossess();	// AI 제어권 회수
	}
	
	// === GamePlay Save ===
	auto* GI = Cast<UReTriGameInstance>(GetGameInstance());
	if (!GI) return; 
	GI->PlayerPlayData.SetKillBoss(1);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	float DieLength = 0.f;
	if (DieMontage)
	{
		DieLength = PlayAnimMontage(DieMontage);
	}
	
	GetWorldTimerManager().SetTimer(
		DeathTimerHandle, this,
		&ADarkMoon::DeathAnimFinished,
		DieLength > 0.2f ? DieLength-0.2f : 0.4f,
		false);
}

void ADarkMoon::DeathAnimFinished()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	
	// ======================== 보스 마무리 추가
	
	// === Boss Drop Item ===
	auto* GI = Cast<UReTriGameInstance>(GetGameInstance());
	if (!GI) { Destroy(); return; }
	auto* MapSub = GI->GetSubsystem<UMapSubSystem>();
	if (!MapSub) { Destroy(); return; }
	
	// 안전장치: BossDropItem이 아직 검색되지 않아 비어있다면, 강제로 한 번 돌려서 세팅합니다.
	if (!MapSub->BossDropItem)
	{
		MapSub->GetRandomAcquiredItemList();
	}
	
	FPlayerSkillData* PickedData = MapSub->BossDropItem;
	// 그래도 비어있다면 데이터 테이블 자체에 boss_drop 행이 없는 것입니다!
	if (!PickedData)
	{
		// UE_LOG(LogTemp, Error, TEXT("[보스 드랍 에러] 데이터테이블(SkillDataTable)에 boss_drop 행이 없거나 불러오지 못했습니다."));
		Destroy();
		return;
	}
	
	// BP에서 ItemClass가 할당되어 있는지 확인
	if (!ItemClass)
	{
		// UE_LOG(LogTemp, Error, TEXT("[보스 드랍 에러] 보스 블루프린트(BP_DarkMoon)의 ItemClass 속성이 None으로 비어있습니다. BP_ItemBase를 할당해주세요!"));
		Destroy();
		return;
	}

	FVector Loc = GetActorLocation() + (GetActorRightVector() * 300.f);
	auto* Item = GetWorld()->SpawnActor<AItemBase>(ItemClass, Loc, FRotator::ZeroRotator);
	if (Item)
	{
		Item->DataInit(*PickedData);

		// Row 이름으로 AbilityClass 찾아서 설정
		FName RowName = FName(*PickedData->SkillID);
		if (TSubclassOf<UAbilityBase>* Found = SkillIDToClassMap.Find(RowName))
		{
			Item->AbilityClass = *Found;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[보스 드랍 에러] 보스 블루프린트의 'SkillIDToClassMap' 에 %s 라는 Key값이 아직 없습니다. 해당 Key를 추가하고 보스 스킬을 연결해주세요!"), *RowName.ToString());
		}
	}
	
	Destroy();
}

void ADarkMoon::StartBattleEvent()
{
	AAIController* aic = Cast<AAIController>(GetController());
	if (!aic) return;

	UStateTreeComponent* StateTreeComp = aic->FindComponentByClass<UStateTreeComponent>();
	
	if (StateTreeComp && StateTreeComp->IsRunning())
	{
		StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(TEXT("Boss.StartBattle")));
		// UE_LOG(LogTemp, Error, TEXT("Battle 시작"));
	}
}

void ADarkMoon::OnSwordCollision()
{
	if (SwordCollision)
	{	
		SwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ADarkMoon::OffSwordCollision()
{
	if (SwordCollision)
	{
		// ZW 콜리젼
		bHasHitTarget = false;
		SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


