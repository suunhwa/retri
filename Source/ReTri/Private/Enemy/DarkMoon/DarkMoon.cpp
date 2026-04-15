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
	
	SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &ADarkMoon::OnSwordOverlap);
	
	if (StatDataTable)
	{
		FEnemyDataTableRow* Data = StatDataTable->FindRow<FEnemyDataTableRow>(EnemyRowName, TEXT("EnemyBegin"));
		
		if (Data)
		{
			MaxHP = Data->MaxHP;
			CurrentHP = MaxHP;
			
			BossSkills = Data->BossSkillsID;
		}
	}
}

void ADarkMoon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 보스 체력 치트키
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::M))
	{
		ReduceBossHP();
	}
}

void ADarkMoon::ReduceBossHP()	// 보스 체력 치트키
{
	CurrentHP -=(MaxHP * 0.2f);
	if (CurrentHP < 0) CurrentHP = 0;
	
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
	
	// 보스 전용, 아직 싸움 시작 전인데 맞았다면? Battle 시작
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
	
	// === Boss Drop Item ===
	auto* GI = Cast<UReTriGameInstance>(GetGameInstance());
	if (!GI) { Destroy(); return; }
	auto* MapSub = GI->GetSubsystem<UMapSubSystem>();
	if (!MapSub) { Destroy(); return; }
	
	if (!MapSub->BossDropItem)
	{
		MapSub->GetRandomAcquiredItemList();
	}
	
	FPlayerSkillData* PickedData = MapSub->BossDropItem;
	if (!PickedData)
	{
		Destroy();
		return;
	}
	
	// BP에서 ItemClass가 할당되어 있는지 확인
	if (!ItemClass)
	{
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