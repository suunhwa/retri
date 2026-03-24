// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/Bullet.h"
#include "Player/Components/HealthComponent.h"
#include "Player/Components/AbilityComponent.h"
#include "Player/Components/StatComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Animation/AnimInstance.h"
#include "InputActionValue.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/ReTriPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));
	SpringArmComp->SetRelativeRotation(FRotator(-65.f, 0.f, 0.f));
	SpringArmComp->TargetArmLength = 2050.f;   
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
	CamComp->SetFieldOfView(55.f);
	
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

	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Inputs/IMC_Player.IMC_Player'"));
	if (TempIMC.Succeeded())
	{
		imc_Player = TempIMC.Object;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		}
	}
}

void APlayerCharacter::OnMove(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	AddMovementInput(FVector::ForwardVector, value.X);
	AddMovementInput(FVector::RightVector, value.Y);
}

void APlayerCharacter::OnAttack(const FInputActionValue& inputValue)
{
	// UE_LOG(LogTemp, Warning, TEXT("OnAttack Called"));
	
	// 클릭한 위치 방향 계산
	AReTriPlayerController* pc = Cast<AReTriPlayerController>(Controller);
	if (!pc) { UE_LOG(LogTemp, Error, TEXT("OnAttack: PC cast failed")); return; }
	// if (!pc) return;
	
	FVector TargetPoint;
	if (!pc->GetMouseWorldPosition(TargetPoint)) { UE_LOG(LogTemp, Error, TEXT("OnAttack: GetMouseWorldPosition failed")); return; }
	// if (!pc->GetMouseWorldPosition(TargetPoint)) return;
	
	FVector Direction = TargetPoint - GetActorLocation();
	Direction.Z = 0.f;
	if (Direction.IsNearlyZero()) { UE_LOG(LogTemp, Error, TEXT("OnAttack: Direction is zero")); return; }
	// if (!Direction.IsNearlyZero()) return;
	Direction.Normalize();
	
	// 클릭한 방향으로 플레이어 회전
	SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));

	// 애니메이션
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AttackMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
	}

	// if (!BulletClass) return;
	if (!BulletClass) { UE_LOG(LogTemp, Error, TEXT("OnAttack: BulletClass is null")); return; }
	
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

	GetWorld()->SpawnActor<ABullet>(
		BulletClass,
		MuzzleLocation,
		Direction.Rotation(),
		SpawnParams
	);
}

void APlayerCharacter::OnTravelerMemory1(const struct FInputActionValue& inputValue)
{
	AbilityComp->TryActivate(EAbilitySlot::TravelerMemory1);
}

void APlayerCharacter::OnSkillQ(const struct FInputActionValue& inputValue)
{
	
}

void APlayerCharacter::OnSkillE(const struct FInputActionValue& inputValue)
{
	
}

void APlayerCharacter::OnTravelerMemory2(const struct FInputActionValue& inputValue)
{
	AbilityComp->TryActivate(EAbilitySlot::TravelerMemory2);
}

float APlayerCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HealthComp->HandleDamage(Damage, EventInstigator);
	return Damage;
}

void APlayerCharacter::OnDash(const struct FInputActionValue& inputValue)
{
	AbilityComp->TryActivate(EAbilitySlot::Dash);
}

void APlayerCharacter::HandleDash(AController* Killer)
{
}
