// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/Projectiles/Bullet.h"
#include "Player/Components/HealthComponent.h"
#include "Player/Components/AbilityComponent.h"
#include "Player/Components/StatComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Animation/AnimInstance.h"
#include "InputActionValue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Player/ReTriPlayerController.h"
#include "ReTriGameData.h"
#include "ReTriGameInstance.h"
#include "Level/Actors/InteractableBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/UI/HPBar.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant'"));
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}
	
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("/Game/Player/Animations/ABP_Player.ABP_Player_C")); 
	if (AnimBP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	}
	
	HealthComp  = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	StatComp    = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	AbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComp"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SpringArmComp->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArmComp->TargetArmLength = 2700.f;   
	SpringArmComp->bUsePawnControlRotation = false; // 카메라 고정
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bDoCollisionTest = false;  
	bUseControllerRotationYaw = false; // 캐릭터 컨트롤러 안 따라감
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	GetCharacterMovement()->bCanWalkOffLedges = false;
	GetCharacterMovement()->NavAgentProps.bCanWalk = true;
	
	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(SpringArmComp);
	CamComp->SetFieldOfView(70.f);
	
	// GD = CreateDefaultSubobject<UReTriGameData>(TEXT("GameData"));
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempMoveInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Move.IA_Move'"));
	if (TempMoveInput.Succeeded())
	{
		ia_Move = TempMoveInput.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempAttackInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Attack.IA_Attack'"));
	if (TempAttackInput.Succeeded())
	{
		ia_Attack = TempAttackInput.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempTM1Input(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_TravelerMemory1.IA_TravelerMemory1'"));
	if (TempTM1Input.Succeeded())
	{
		ia_TravelerMemory1 = TempTM1Input.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempSkillQInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_SkillQ.IA_SkillQ'"));
	if (TempSkillQInput.Succeeded())
	{
		ia_SkillQ = TempSkillQInput.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempSkillEInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_SkillE.IA_SkillE'"));
	if (TempSkillEInput.Succeeded())
	{
		ia_SkillE = TempSkillEInput.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempTM2Input(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_TravelerMemory2.IA_TravelerMemory2'"));
	if (TempTM2Input.Succeeded())
	{
		ia_TravelerMemory2 = TempTM2Input.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempDashInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Dash.IA_Dash'"));
	if (TempDashInput.Succeeded())
	{
		ia_Dash = TempDashInput.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempInteractionInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Interaction.IA_Interaction'"));
	if (TempInteractionInput.Succeeded())
	{
		ia_Interaction = TempInteractionInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Inputs/IMC_Player.IMC_Player'"));
	if (TempIMC.Succeeded())
	{
		imc_Player = TempIMC.Object;
	}
	
	HPBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarComp"));
	HPBarComp->SetupAttachment(GetMesh());
	HPBarComp->SetRelativeLocation(FVector(0.f, 0.f, 250.f)); // 머리 위 높이 조절
	HPBarComp->SetWidgetSpace(EWidgetSpace::World); // 항상 카메라 향함
	HPBarComp->SetDrawSize(FVector2D(150.f, 20.f));  // 크기 조절
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// OnDeath 델리게이트 바인딩
	HealthComp->OnDeath.AddDynamic(this, &APlayerCharacter::HandleDeath);
	
	if (UHPBar* HPWidget = Cast<UHPBar>(HPBarComp->GetUserWidgetObject()))
	{
		HealthComp->OnHPChanged.AddDynamic(HPWidget, &UHPBar::OnHPChanged);
	}
	
	// GD->DebugStat();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsCombat)
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - LastCombatTime >= CombatExitDelay)
		{
			bIsCombat = false;
		}
	}

	/*// 항상 마우스 커서 방향을 바라봄
	AReTriPlayerController* PC = Cast<AReTriPlayerController>(Controller);
	if (PC)
	{
		FVector TargetPoint;
		if (PC->GetMouseWorldPosition(TargetPoint))
		{
			FVector Dir = TargetPoint - GetActorLocation();
			Dir.Z = 0.f;
			if (!Dir.IsNearlyZero())
			{
				SetActorRotation(FRotator(0.f, Dir.Rotation().Yaw, 0.f));
			}
		}
	}*/
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 입력 바인딩
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(imc_Player, 0);
		}
		
		auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (playerInput)
		{
			playerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMove);
			playerInput->BindAction(ia_Attack, ETriggerEvent::Started, this, &APlayerCharacter::OnAttack);
			playerInput->BindAction(ia_TravelerMemory1, ETriggerEvent::Started, this, &APlayerCharacter::OnTravelerMemory1);
			playerInput->BindAction(ia_SkillQ, ETriggerEvent::Started, this, &APlayerCharacter::OnSkillQ);
			playerInput->BindAction(ia_SkillE, ETriggerEvent::Started, this, &APlayerCharacter::OnSkillE);
			playerInput->BindAction(ia_TravelerMemory2, ETriggerEvent::Started, this, &APlayerCharacter::OnTravelerMemory2);
			playerInput->BindAction(ia_Dash, ETriggerEvent::Started, this, &APlayerCharacter::OnDash);
			playerInput->BindAction(ia_Interaction, ETriggerEvent::Started, this, &APlayerCharacter::OnInteraction);
		}
	}
}

void APlayerCharacter::OnMove(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	AddMovementInput(FVector::ForwardVector, value.X);
	AddMovementInput(FVector::RightVector, value.Y);
}

void APlayerCharacter::EnterCombat()
{
	bIsCombat = true;
	LastCombatTime = GetWorld()->GetTimeSeconds();
}

void APlayerCharacter::OnAttack(const FInputActionValue& inputValue)
{
	if (!bCanAttack) return;
	EnterCombat();
	
	// UE_LOG(LogTemp, Warning, TEXT("OnAttack Called"));
	
	// 클릭한 위치 방향 계산
	AReTriPlayerController* pc = Cast<AReTriPlayerController>(Controller);
	if (!pc) return;
	
	FVector TargetPoint;
	if (!pc->GetMouseWorldPosition(TargetPoint)) return;
	
	FVector Direction = TargetPoint - GetActorLocation();
	Direction.Z = 0.f;
	if (Direction.IsNearlyZero()) return;
	Direction.Normalize();
	
	// 방향 검증 통과 후 쿨타임
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APlayerCharacter::ResetAttack, AttackInterval, false);
	
	// 클릭한 방향으로 플레이어 회전
	SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));

	// 애니메이션
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AttackMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
	}

	if (!BulletClass) return;
	
	// 4탄마다 강화탄
	AttackCount++;
    bool bIsEnhancedShot = (AttackCount >= 4);
	
	if (bIsEnhancedShot)
	{
		AttackCount = 0;
	}
	
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("weapon_muzzle"));
	// UE_LOG(LogTemp, Warning, TEXT("Muzzle: %s"), *MuzzleLocation.ToString());
	/*FVector Direction = GetActorForwardVector();
	Direction.Z = 0.f;
	Direction.Normalize();*/

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	// SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABullet* SpawnedBullet = GetWorld()->SpawnActor<ABullet>(
		BulletClass,
		MuzzleLocation,
		Direction.Rotation(),
		SpawnParams
	);
	
	if (SpawnedBullet && bIsEnhancedShot)
	{
		SpawnedBullet->SetBulletDamage(SpawnedBullet->GetBulletDamage() * EnhancedShotMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("[Attack] 강화탄 Damage: %.1f"), SpawnedBullet->GetBulletDamage());
		
		if (EnhancedShotEffect)
		{
			if (UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAttached(
				EnhancedShotEffect,
				SpawnedBullet->GetRootComponent(),
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true
			))
			{
				NC->SetWorldScale3D(FVector(EnhancedShotEffectScale));
			}
		}
		if (EnhancedShotParticle)
		{
			if (UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAttached(
				EnhancedShotParticle,
				SpawnedBullet->GetRootComponent()
			))
			{
				PSC->SetWorldScale3D(FVector(EnhancedShotEffectScale));
			}
		}
	}
	else if (SpawnedBullet)
	{
		UE_LOG(LogTemp, Log, TEXT("[Attack] 일반탄 %d/4. Damage: %.1f"), AttackCount, SpawnedBullet->GetBulletDamage());
	}
}

void APlayerCharacter::ResetAttack()
{
	bCanAttack = true;
}

void APlayerCharacter::OnTravelerMemory1(const struct FInputActionValue& inputValue)
{
	UE_LOG(LogTemp, Warning, TEXT("RMB pressed"));
	EnterCombat();
	AbilityComp->TryActivate(EAbilitySlot::TravelerMemory1);
}

void APlayerCharacter::OnSkillQ(const struct FInputActionValue& inputValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Q pressed"));
	EnterCombat();
	AbilityComp->TryActivate(EAbilitySlot::SkillQ);
}

void APlayerCharacter::OnSkillE(const struct FInputActionValue& inputValue)
{
	UE_LOG(LogTemp, Warning, TEXT("E pressed"));
	EnterCombat();
	AbilityComp->TryActivate(EAbilitySlot::SkillE);
}

void APlayerCharacter::OnTravelerMemory2(const struct FInputActionValue& inputValue)
{
	UE_LOG(LogTemp, Warning, TEXT("R pressed"));
	EnterCombat();
	AbilityComp->TryActivate(EAbilitySlot::TravelerMemory2);
}

void APlayerCharacter::OnInteraction(const struct FInputActionValue& inputValue)
{
	Interaction();
}

float APlayerCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
                                   AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthComp->HandleDamage(Damage, EventInstigator);

	UE_LOG(LogTemp, Warning, TEXT("[Hit] Damage: %.1f | HP: %.1f / %.1f"),
		Damage, HealthComp->GetCurrentHP(), HealthComp->GetMaxHP());

	if (!HealthComp->IsDead() && HitMontage)
	{
		PlayAnimMontage(HitMontage);
	}

	return Damage;
}

void APlayerCharacter::OnDash(const struct FInputActionValue& inputValue)
{
	AbilityComp->TryActivate(EAbilitySlot::Dash);
}

void APlayerCharacter::HandleDeath(AController* Killer)
{
	UE_LOG(LogTemp, Warning, TEXT("[Death] 플레이어 사망"));
	
	// 입력 차단
	DisableInput(Cast<APlayerController>(Controller));

	// 이동 정지
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	// 충돌 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 사망 몽타주 재생
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}

	// 일정 시간 후 레벨 재시작
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, [this]()
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}, 3.f, false);
}

void APlayerCharacter::HoverInteractable()
{
	// 감지하고자 하는 오브젝트 타입들을 배열에 담기.
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); // Interaction
	
	FHitResult HitResult;
	bool bHit = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(ObjectTypes, false, HitResult);
	if (bHit && HitResult.GetActor()->Implements<UInteractableInterface>())
	{
		IInteractableInterface::Execute_Hover(HitResult.GetActor());
	}
}

void APlayerCharacter::Interaction()
{
	// F 키를 눌렀을 때 오브젝트 들과 상호작용 할 수 있도록 
	
	// 감지하고자 하는 오브젝트 타입
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); // Interaction
	
	// 제외할 Actors 
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), 42.f, 16, FColor::Red);
	
	// 근처에 Interaction Object가 있는지 감지 
	TArray<AActor*> OutActors;
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 42.f, ObjectTypes, AActor::StaticClass(), IgnoreActors,OutActors);
	
	if (!bHit) return;
	for (AActor* Actor : OutActors)
	{
		AInteractableBase* Interact = Cast<AInteractableBase>(Actor);
		if (Interact)
		{
			IInteractableInterface::Execute_Interact(Interact);
			
			auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
			GI->GameData->DebugStat();
			// GD->DebugStat();
		}
	}
}
