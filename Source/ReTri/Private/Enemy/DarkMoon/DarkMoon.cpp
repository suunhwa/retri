// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DarkMoon/DarkMoon.h"

#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"


ADarkMoon::ADarkMoon()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Skins/GlacialEmpress/Meshes/Aurora_GlacialEmpress.Aurora_GlacialEmpress'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -180.0f), FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	}
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 196.0f);
	
	
	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	SwordCollision->SetupAttachment(GetMesh(), TEXT("SwordSocket"));
	SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordCollision->SetRelativeLocation(FVector(0.000000,65.000000,0.000000));
	SwordCollision->SetBoxExtent(FVector(12.173687,51.076195,5.828096));
	SwordCollision->SetGenerateOverlapEvents(true);
}

void ADarkMoon::BeginPlay()
{
	Super::BeginPlay();
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
			
			UE_LOG(LogTemp, Warning, TEXT("%s 등장 스킬 %d종류 장착"), *EnemyRowName.ToString(), BossSkills.Num());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%s행 못 찾음"), *EnemyRowName.ToString());
		}
	}
	
}

void ADarkMoon::SetSwordCollisionEnabled(bool bEnabled)
{
	if (bEnabled)
		SwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADarkMoon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AAIController* aic = Cast<AAIController>(GetController()))
	{
		UActorComponent* STComp = aic->GetComponentByClass(UStateTreeComponent::StaticClass());
		UStateTreeComponent* StateTreeComp = Cast<UStateTreeComponent>(STComp);
		
		if (StateTreeComp)
		{
			StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(TEXT("Boss.StartBattle")));
			
			UE_LOG(LogTemp, Error, TEXT("Battle 시작"))
			
			GetCapsuleComponent()->OnComponentBeginOverlap.RemoveDynamic(this, &ADarkMoon::OnOverlapBegin);
		}
	}
}

void ADarkMoon::OnSwordOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// 대미지
		UE_LOG(LogTemp, Warning, TEXT("%s를 때렸다!!!"), *OtherActor->GetName());
	}
}

float ADarkMoon::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// if (AAIController* aic = Cast<AAIController>(GetController()))
	// {
	// 	UActorComponent* STComp = aic->GetComponentByClass(UStateTreeComponent::StaticClass());
	// 	UStateTreeComponent* StateTreeComp = Cast<UStateTreeComponent>(STComp);
	// 	
	// 	if (StateTreeComp)
	// 	{
	// 		StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(TEXT("Boss.StartBattle")));
	// 		
	// 		UE_LOG(LogTemp, Error, TEXT("Battle 시작"))
	// 	}
	// }
	
	return ActualDamage;
}
