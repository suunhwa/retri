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
#include "ReTriGameInstance.h"
#include "Item/Interface/SkillItemInterface.h"
#include "Level/Actors/InteractableBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Level/Actors/FloatingUIActor.h"
#include "Level/Actors/BossDropItem.h"
#include "Player/UI/HPBar.h"
#include "Player/UI/PlayerHUD.h"
#include "Level/UI/GameOverUI.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant'"));
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

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
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

	ConstructorHelpers::FObjectFinder<UInputAction> TempMoveInput(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Move.IA_Move'"));
	if (TempMoveInput.Succeeded())
	{
		ia_Move = TempMoveInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempAttackInput(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Attack.IA_Attack'"));
	if (TempAttackInput.Succeeded())
	{
		ia_Attack = TempAttackInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempTM1Input(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_TravelerMemory1.IA_TravelerMemory1'"));
	if (TempTM1Input.Succeeded())
	{
		ia_TravelerMemory1 = TempTM1Input.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempSkillQInput(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_SkillQ.IA_SkillQ'"));
	if (TempSkillQInput.Succeeded())
	{
		ia_SkillQ = TempSkillQInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempSkillEInput(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_SkillE.IA_SkillE'"));
	if (TempSkillEInput.Succeeded())
	{
		ia_SkillE = TempSkillEInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempTM2Input(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_TravelerMemory2.IA_TravelerMemory2'"));
	if (TempTM2Input.Succeeded())
	{
		ia_TravelerMemory2 = TempTM2Input.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempDashInput(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Dash.IA_Dash'"));
	if (TempDashInput.Succeeded())
	{
		ia_Dash = TempDashInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempInteractionInput(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Interaction.IA_Interaction'"));
	if (TempInteractionInput.Succeeded())
	{
		ia_Interaction = TempInteractionInput.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempSalvageInput(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/ia_Salvage.ia_Salvage'"));
	if (TempSalvageInput.Succeeded())
	{
		ia_Salvage = TempSalvageInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempHPInput(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_HP.IA_HP'"));
	if (TempHPInput.Succeeded())
	{
		ia_HP = TempHPInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Inputs/IMC_Player.IMC_Player'"));
	if (TempIMC.Succeeded())
	{
		imc_Player = TempIMC.Object;
	}

	HPBarComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarComp"));
	HPBarComp->SetupAttachment(GetMesh());
	HPBarComp->SetRelativeLocation(FVector(0.f, 0.f, 300.f)); // 머리 위 높이 조절
	// HPBarComp->SetupAttachment(GetMesh(), TEXT("HP_Bar"));
	HPBarComp->SetWidgetSpace(EWidgetSpace::Screen); // 항상 카메라 향함
	HPBarComp->SetDrawSize(FVector2D(100.f, 15.f)); // 크기 조절
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// CDO에서 실행 방지
	if (HasAnyFlags(RF_ClassDefaultObject)) return;

	// game instance에 컴포넌트 등록
	if (UReTriGameInstance* GI = Cast<UReTriGameInstance>(GetGameInstance()))
	{
		GI->StatComp = StatComp;
		GI->HealthComp = HealthComp;
	}

	// OnDeath 델리게이트 바인딩
	HealthComp->OnDeath.AddDynamic(this, &APlayerCharacter::HandleDeath);
	StatComp->OnLevelUp.AddDynamic(this, &APlayerCharacter::HandleLevelUp);
	// 메인 메뉴에서는 UI 표시 안 함
	const bool bIsMainMenu = GetWorld()->GetMapName().Contains(TEXT("Lv_Main"));
	if (bIsMainMenu)
	{
		HPBarComp->SetVisibility(false);
	}
	if (!bIsMainMenu)
	{
		const bool bIsStartMap = GetWorld()->GetMapName().Contains(TEXT("LV_StartMap"));
		if (bIsStartMap && IntroSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), IntroSound, GetActorLocation());

		const bool bIsBossMap = GetWorld()->GetMapName().Contains(TEXT("Lv_BossMap"));
		if (bIsBossMap && BossMapIntroSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BossMapIntroSound, GetActorLocation());

		HPBarComp->InitWidget();
		if (UHPBar* HPWidget = Cast<UHPBar>(HPBarComp->GetUserWidgetObject()))
		{
			// UE_LOG(LogTemp, Warning, TEXT("[HPBar] 위젯 바인딩 성공"));
			HealthComp->OnHPChanged.AddDynamic(HPWidget, &UHPBar::OnHPChanged);
			HPWidget->OnHPChanged(HealthComp->GetCurrentHP(), HealthComp->GetMaxHP());
		}

		// PlayerHUD 생성 (화면 좌하단)
		if (PlayerHUDClass)
		{
			PlayerHUDWidget = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDClass);
			if (PlayerHUDWidget)
			{
				PlayerHUDWidget->AddToViewport();
				HealthComp->OnHPChanged.AddDynamic(PlayerHUDWidget, &UPlayerHUD::OnHPChanged);
				StatComp->OnStatChanged.AddDynamic(PlayerHUDWidget, &UPlayerHUD::OnStatChanged);
				StatComp->OnExpChanged.AddDynamic(PlayerHUDWidget, &UPlayerHUD::OnExpChanged);
				// 초기값 표시
				PlayerHUDWidget->OnHPChanged(HealthComp->GetCurrentHP(), HealthComp->GetMaxHP());
				const int32 RequiredExp = StatComp->GetRequiredExpForLevel(StatComp->GetCurrentLevel() + 1);
				PlayerHUDWidget->OnExpChanged(StatComp->GetCurrentExp(), RequiredExp, StatComp->GetCurrentLevel());

				// 초기 재화 표시
				const FPlayerStatInfo InitStat = StatComp->GetStatInfo();
				PlayerHUDWidget->OnStatChanged(EStatTypes::Gold, InitStat.Gold);
				PlayerHUDWidget->OnStatChanged(EStatTypes::DreamDust, InitStat.DreamDust);

				// 스킬바 초기화
				PlayerHUDWidget->InitSkillBar(AbilityComp);
			}
		}
	}
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
			
			// 전투 이탈 시 공격 카운터 초기화 → 패시브 링 사라짐
			if (AttackCount > 0)
			{
				AttackCount = 0;
				OnAttackCountChanged.Broadcast(0);
			}
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
			playerInput->BindAction(ia_TravelerMemory1,
			                        ETriggerEvent::Started,
			                        this,
			                        &APlayerCharacter::OnTravelerMemory1);
			playerInput->BindAction(ia_SkillQ, ETriggerEvent::Started, this, &APlayerCharacter::OnSkillQ);
			playerInput->BindAction(ia_SkillE, ETriggerEvent::Started, this, &APlayerCharacter::OnSkillE);
			playerInput->BindAction(ia_TravelerMemory2,
			                        ETriggerEvent::Started,
			                        this,
			                        &APlayerCharacter::OnTravelerMemory2);
			playerInput->BindAction(ia_Dash, ETriggerEvent::Started, this, &APlayerCharacter::OnDash);
			playerInput->BindAction(ia_Interaction, ETriggerEvent::Started, this, &APlayerCharacter::OnInteraction);
			playerInput->BindAction(ia_Interaction, ETriggerEvent::Started, this, &APlayerCharacter::OnPickUp);
			playerInput->BindAction(ia_Salvage, ETriggerEvent::Started, this, &APlayerCharacter::OnSalvage);
			playerInput->BindAction(ia_HP, ETriggerEvent::Started, this, &APlayerCharacter::OnCheatHP);
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

	// 리셋 전에 브로드캐스트 → 4타째에 100% 표시 후 다음 1타에서 리셋
	OnAttackCountChanged.Broadcast(AttackCount);
	
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

	if (SpawnedBullet)
	{
		SpawnedBullet->SetBulletDamage(StatComp->GetAttackPower());
	}

	if (SpawnedBullet && bIsEnhancedShot)
	{
		SpawnedBullet->SetBulletDamage(StatComp->GetAttackPower() * EnhancedShotMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("[Attack] 강화탄 Damage: %.1f"), SpawnedBullet->GetBulletDamage());

		if (EnhancedShotCS)
		{
			if (APlayerController* PC = Cast<APlayerController>(Controller))
			{
				PC->ClientStartCameraShake(EnhancedShotCS);
			}
		}

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

		// 총구에 남아있는 이펙트 (총알과 별개로 제자리에 터짐)
		if (EnhancedMuzzleParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), EnhancedMuzzleParticle, MuzzleLocation,
				Direction.Rotation(), FVector(EnhancedShotEffectScale)
			);
		}
	}
	else if (SpawnedBullet)
	{
		if (NormalShotCS)
		{
			if (APlayerController* PC = Cast<APlayerController>(Controller))
			{
				PC->ClientStartCameraShake(NormalShotCS);
			}
		}
		// UE_LOG(LogTemp, Log, TEXT("[Attack] 일반탄 %d/4. Damage: %.1f"), AttackCount, SpawnedBullet->GetBulletDamage());
	}
}

void APlayerCharacter::ResetAttack()
{
	bCanAttack = true;
}

void APlayerCharacter::OnTravelerMemory1(const struct FInputActionValue& inputValue)
{
	// UE_LOG(LogTemp, Warning, TEXT("RMB pressed"));
	EnterCombat();
	AbilityComp->TryActivate(EAbilitySlot::TravelerMemory1);
}

void APlayerCharacter::OnSkillQ(const struct FInputActionValue& inputValue)
{
	// UE_LOG(LogTemp, Warning, TEXT("Q pressed"));
	EnterCombat();
	AbilityComp->TryActivate(EAbilitySlot::SkillQ);
}

void APlayerCharacter::OnSkillE(const struct FInputActionValue& inputValue)
{
	// UE_LOG(LogTemp, Warning, TEXT("E pressed"));
	EnterCombat();
	AbilityComp->TryActivate(EAbilitySlot::SkillE);
}

void APlayerCharacter::OnTravelerMemory2(const struct FInputActionValue& inputValue)
{
	// UE_LOG(LogTemp, Warning, TEXT("R pressed"));
	EnterCombat();
	AbilityComp->TryActivate(EAbilitySlot::TravelerMemory2);
}

void APlayerCharacter::OnInteraction(const struct FInputActionValue& inputValue)
{
	Interaction();
}

float APlayerCharacter::TakeDamage(float DamageAmount,
                                   const FDamageEvent& DamageEvent,
                                   AController* EventInstigator,
                                   AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthComp->HandleDamage(Damage, EventInstigator);
	
	// === Damage UI ===
	if (auto* GI = Cast<UReTriGameInstance>(GetGameInstance()))
	{
		// === GamePlay Save ===
		GI->PlayerPlayData.SetGetDamage(Damage);
		
		if (GI->FloatingUIActorClass)
		{
			AFloatingUIActor* DamageText = GetWorld()->SpawnActor<AFloatingUIActor>(
				GI->FloatingUIActorClass, 
				GetActorLocation(),
				FRotator::ZeroRotator
			);
				
			FString DmgString = FString::Printf(TEXT("%d"), FMath::RoundToInt(Damage));
			DamageText->ShowScaleUI(FText::FromString(DmgString), FLinearColor::Red);
		}
	}

	UE_LOG(LogTemp,
	       Warning,
	       TEXT("[Hit] Damage: %.1f | HP: %.1f / %.1f"),
	       Damage,
	       HealthComp->GetCurrentHP(),
	       HealthComp->GetMaxHP());

	if (!HealthComp->IsDead())
	{
		if (HitSound && bCanPlayHitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
			bCanPlayHitSound = false;
			GetWorldTimerManager().SetTimer(HitSoundTimerHandle, [this]()
			{
				bCanPlayHitSound = true;
			}, HitSoundCooldown, false);
		}

		const float HPRatio = HealthComp->GetCurrentHP() / HealthComp->GetMaxHP();
		if (HPRatio < 0.3f && !bLowHPSoundPlayed && LowHPSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), LowHPSound, GetActorLocation());
			bLowHPSoundPlayed = true;
		}

		if (HitMontage)
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

	if (DeathSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());

	// 입력 차단
	DisableInput(Cast<APlayerController>(Controller));

	// 이동 정지
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	// 충돌 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 사망 애니메이션 종료 후 게임 일시정지
	FTimerHandle DeathPauseHandle;
	GetWorldTimerManager().SetTimer(DeathPauseHandle, [this]()
	{
		UGameplayStatics::SetGamePaused(this, true);
		
		// === GameOverUI === 
		if (GameOverUIClass != nullptr)
		{
			GameOverUI = CreateWidget<UGameOverUI>(GetWorld(), GameOverUIClass);
		}
		if (GameOverUI != nullptr)
		{
			GameOverUI->SetUIData();
			GameOverUI->AddToViewport(10);
		}
		
	}, DeathAnimDuration, false);
	
}

void APlayerCharacter::HoverInteractable()
{
	// 감지하고자 하는 오브젝트 타입들을 배열에 담기.
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)); // Interaction

	FHitResult HitResult;
	bool bHit = GetWorld()->GetFirstPlayerController()->
	                        GetHitResultUnderCursorForObjects(ObjectTypes, false, HitResult);
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
	bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
	                                                      GetActorLocation(),
	                                                      42.f,
	                                                      ObjectTypes,
	                                                      AActor::StaticClass(),
	                                                      IgnoreActors,
	                                                      OutActors);

	if (!bHit) return;
	for (AActor* Actor : OutActors)
	{
		if (Actor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			IInteractableInterface::Execute_Interact(Actor);

			auto* GI = Cast<UReTriGameInstance>(GetWorld()->GetGameInstance());
			GI->DebugStat();

			return;
		}
	}
}

void APlayerCharacter::OnPickUp(const struct FInputActionValue& inputValue)
{
	const float PickUpRadius = 200.f;

	// ISkillItemInterface 구현 액터 전체에서 거리 체크 (콜리전 채널 무시)
	TArray<AActor*> AllItems;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USkillItemInterface::StaticClass(), AllItems);

	for (AActor* Actor : AllItems)
	{
		if (FVector::Dist(GetActorLocation(), Actor->GetActorLocation()) <= PickUpRadius)
		{
			ISkillItemInterface::Execute_Acquire(Actor);
			return; // 가장 먼저 찾은 아이템 하나만 처리
		}
	}
	
	// 감지 안됨
	/*UE_LOG(LogTemp, Warning, TEXT("[OnPickUp] 키 입력 감지"));

	// 스킬 아이템 픽업 (ECC_GameTraceChannel6 — ItemBase 콜리전 채널)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel6));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
											  GetActorLocation(),
											  200.f,
											  ObjectTypes,
											  AActor::StaticClass(),
											  IgnoreActors,
											  OutActors);

	UE_LOG(LogTemp, Warning, TEXT("[OnPickUp] 감지된 액터 수: %d"), OutActors.Num());

	for (AActor* Actor : OutActors)
	{
		if (Actor->GetClass()->ImplementsInterface(USkillItemInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("[OnPickUp] 아이템 발견, Acquire 호출: %s"), *Actor->GetName());
			ISkillItemInterface::Execute_Acquire(Actor);
			return;
		}
	}*/
}

void APlayerCharacter::OnSalvage(const struct FInputActionValue& inputValue)
{
	TArray<AActor*> AllItems;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USkillItemInterface::StaticClass(), AllItems);

	const float SalvageRadius = 300.f;
	for (AActor* Actor : AllItems)
	{
		if (FVector::Dist(GetActorLocation(), Actor->GetActorLocation()) <= SalvageRadius)
		{
			ISkillItemInterface::Execute_Hold(Actor, this);
			return; 
		}
	}
}

void APlayerCharacter::OnCheatHP(const FInputActionValue& inputValue)
{
	HealthComp->Heal(50.f);
}

void APlayerCharacter::DebugAddExp(int32 Amount)
{
	StatComp->AddExp(Amount);
}

void APlayerCharacter::HandleLevelUp(int32 NewLevel)
{
	UNiagaraSystem* EffectToPlay = nullptr;
	USoundBase* SoundToPlay = nullptr;

	if (NewLevel >= UStatComponent::MaxLevel)
	{
		EffectToPlay = MaxLevelEffect;
		SoundToPlay = MaxLevelSound;
	}
	else if (NewLevel >= 5)
	{
		EffectToPlay = MidLevelEffect;
		SoundToPlay = MidLevelSound;
	}
	else
	{
		EffectToPlay = LevelUpEffect;
		SoundToPlay = LevelUpSound;
	}

	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation());
	}

	if (EffectToPlay)
	{
		UNiagaraComponent* NC = UNiagaraFunctionLibrary::SpawnSystemAttached(
			EffectToPlay, GetMesh(), LevelUpSocketName,
			FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget, true
		);

		if (NC)
		{
			NC->SetWorldScale3D(LevelUpEffectScale);

			FTimerHandle FadeHandle;
			TWeakObjectPtr<UNiagaraComponent> WeakNC(NC);
			GetWorld()->GetTimerManager().SetTimer(FadeHandle, [WeakNC]()
			{
				if (WeakNC.IsValid())
				{
					WeakNC->DeactivateImmediate();
				}
			}, LevelUpEffectDuration, false);
		}
	}
}
