#include "Human.h"
#include "HumanController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AHuman::AHuman()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComp->SetSimulatePhysics(false);
	SetRootComponent(CapsuleComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	SkeletalMeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	Pitch = 0.f;
	MoveInput = FVector2D::ZeroVector;
	LookInput = FVector2D::ZeroVector;
	MoveSpeed = 600.f;
}

void AHuman::BeginPlay()
{
	Super::BeginPlay();
}

void AHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector LocalDelta(
		MoveInput.X * MoveSpeed * DeltaTime,
		MoveInput.Y * MoveSpeed * DeltaTime,
		0.f );
	AddActorLocalOffset(LocalDelta, true);
}

void AHuman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AHumanController* PlayerController = Cast<AHumanController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AHuman::Move);
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &AHuman::StopMove);
			}
			if (PlayerController->LookAction)
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AHuman::Look);
		}
	}
}

void AHuman::Move(const FInputActionValue& Value) { MoveInput = Value.Get<FVector2D>(); }

void AHuman::StopMove(const FInputActionValue& Value) { MoveInput = FVector2D::ZeroVector; }

void AHuman::Look(const FInputActionValue& Value)
{
	LookInput = Value.Get<FVector2D>();

	// Yaw: Pawn 회전
	AddActorLocalRotation(FRotator(0.f, LookInput.X, 0.f));

	// Pitch: SpringArm 회전
	Pitch = FMath::Clamp(Pitch + LookInput.Y, -80.f, 80.f);
	SpringArmComp->SetRelativeRotation(FRotator(Pitch, 0.f, 0.f));
}