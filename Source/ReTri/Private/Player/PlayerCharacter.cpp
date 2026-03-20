// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
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
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	SpringArmComp->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
	SpringArmComp->TargetArmLength = 1200.f;   
	SpringArmComp->bUsePawnControlRotation = false; // 카메라 고정
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bDoCollisionTest = false;  
	bUseControllerRotationYaw = false; // 캐릭터 컨트롤러 안 따라감
	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향 회전 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	
	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(SpringArmComp);
	CamComp->SetFieldOfView(75.f);
	
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
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempSecondaryInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_Secondary.IA_Secondary'"));
	if (TempSecondaryInput.Succeeded())
	{
		ia_Secondary = TempSecondaryInput.Object;
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
	
	ConstructorHelpers::FObjectFinder<UInputAction> TempSkillRInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Inputs/IA_SkillR.IA_SkillR'"));
	if (TempSkillRInput.Succeeded())
	{
		ia_SkillR = TempSkillRInput.Object;
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

	/*FVector p0 = GetActorLocation();
	FVector vt = direction * speed * DeltaTime;
	SetActorLocation(p0 + vt);
	direction = FVector::ZeroVector;*/
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
			playerInput->BindAction(ia_Attack, ETriggerEvent::Triggered, this, &APlayerCharacter::OnAttack);
			playerInput->BindAction(ia_Secondary, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSecondary);
			playerInput->BindAction(ia_SkillQ, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSkillQ);
			playerInput->BindAction(ia_SkillE, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSkillE);
			playerInput->BindAction(ia_SkillR, ETriggerEvent::Triggered, this, &APlayerCharacter::OnSkillR);
		}
	}
}

void APlayerCharacter::OnMove(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	AddMovementInput(FVector::ForwardVector, value.X);
	AddMovementInput(FVector::RightVector, value.Y);
	/*direction.X = value.X;
	direction.Y = value.Y;*/
}

void APlayerCharacter::OnAttack(const struct FInputActionValue& inputValue)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AttackMontage || !AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Play(AttackMontage);
	
	if (!BulletClass) return;

	AReTriPlayerController* PC = Cast<AReTriPlayerController>(Controller);
	if (!PC) return;

	FVector TargetPoint;
	if (!PC->GetMouseWorldPosition(TargetPoint)) return;

	FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("weapon_muzzle"));
	FVector Direction = (TargetPoint - MuzzleLocation);
	Direction.Z = 0.f;
	Direction.Normalize();

	// 캐릭터 커서 방향으로 회전
	SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, Direction.Rotation(), SpawnParams);
}

void APlayerCharacter::OnSecondary(const struct FInputActionValue& inputValue)
{
	
}

void APlayerCharacter::OnSkillQ(const struct FInputActionValue& inputValue)
{
	
}

void APlayerCharacter::OnSkillE(const struct FInputActionValue& inputValue)
{
	
}

void APlayerCharacter::OnSkillR(const struct FInputActionValue& inputValue)
{
	
}

void APlayerCharacter::OnDash(const struct FInputActionValue& inputValue)
{
}

void APlayerCharacter::HandleDash(AController* Killer)
{
}
