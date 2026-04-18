#include "Drone.h"
#include "DroneController.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

ADrone::ADrone()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);
    BoxComp->SetSimulatePhysics(false);

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(BoxComp);

    Wing1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing1"));
    Wing1->SetupAttachment(MeshComp);
    Wing2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing2"));
    Wing2->SetupAttachment(MeshComp);
    Wing3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing3"));
    Wing3->SetupAttachment(MeshComp);
    Wing4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wing4"));
    Wing4->SetupAttachment(MeshComp);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(BoxComp);
    SpringArmComp->TargetArmLength = 200.f;
    SpringArmComp->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    MoveInput = FVector2D::ZeroVector;
    LookInput = FVector2D::ZeroVector;
    VerticalInput = 0.f;
    RollInput = 0.f;
    VerticalVelocity = 0.f;
    bGravityEnabled = true;
    bIsGrounded = true;
    MoveSpeed = 600.f;
    RotateSpeed = 100.f;
    SpeedFactor = 0.5f;
    InterpSpeed = 5.f;
    Gravity = -980.f;
    GroundTraceDistance = 18.5f;
    WingRotateSpeed = 2880.f;
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();
}

void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    HandleMovement(DeltaTime);
    HandleRotation(DeltaTime);
    HandleWingRotation(DeltaTime);

    MoveInput = FVector2D::ZeroVector;
    LookInput = FVector2D::ZeroVector;
    VerticalInput = 0.f;
    RollInput = 0.f;
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ADroneController* DroneController = Cast<ADroneController>(GetController()))
        {
            if (DroneController->MoveAction)
                EnhancedInput->BindAction(DroneController->MoveAction, ETriggerEvent::Triggered, this, &ADrone::Move);
            if (DroneController->MoveUpAction)
                EnhancedInput->BindAction(DroneController->MoveUpAction, ETriggerEvent::Triggered, this, &ADrone::MoveUp);
            if (DroneController->LookAction)
                EnhancedInput->BindAction(DroneController->LookAction, ETriggerEvent::Triggered, this, &ADrone::Look);
            if (DroneController->RollAction)
                EnhancedInput->BindAction(DroneController->RollAction, ETriggerEvent::Triggered, this, &ADrone::Roll);
            if (DroneController->ToggleGravityAction)
                EnhancedInput->BindAction(DroneController->ToggleGravityAction, ETriggerEvent::Started, this, &ADrone::ToggleGravity);
        }
    }
}

void ADrone::HandleMovement(float DeltaTime)
{
    // 지상과 공중 속도 조절
    float CurrentMoveSpeed = bIsGrounded ? MoveSpeed : MoveSpeed * SpeedFactor;

    FVector HorizontalDelta = (
        GetActorForwardVector() * MoveInput.X + 
        GetActorRightVector() * MoveInput.Y
        ) * CurrentMoveSpeed * DeltaTime;
    AddActorWorldOffset(HorizontalDelta, true);

    FVector VerticalDelta = FVector::ZeroVector;
    HandleGravity(DeltaTime, VerticalDelta);

    if (!bGravityEnabled && VerticalInput != 0.f)
        VerticalDelta += GetActorUpVector() * VerticalInput * CurrentMoveSpeed * DeltaTime;

    AddActorWorldOffset(VerticalDelta, true);
}

void ADrone::HandleGravity(float DeltaTime, FVector& Delta)
{
    if (!bGravityEnabled)
    {
        bIsGrounded = false;
        return;
    }

    VerticalVelocity += Gravity * DeltaTime;
    Delta.Z += VerticalVelocity * DeltaTime;

    FVector Start = GetActorLocation();
    FVector End = Start + FVector(0.f, 0.f, -GroundTraceDistance);
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        bIsGrounded = true;
        VerticalVelocity = 0.f;
        Delta.Z = 0.f; // 지면 관통 방지
    }
    else
        bIsGrounded = false;
}

void ADrone::HandleRotation(float DeltaTime)
{
    FQuat CurrentQuat = GetActorQuat();

    if (bGravityEnabled)
    {
        // 지상에서는 Z축만 계산(월드 기준)
        FQuat YawQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(LookInput.X * RotateSpeed * DeltaTime));
        SetActorRotation(YawQuat * CurrentQuat);

        // 중력을 켰을 때, 원래의 각도로 돌아가는 계산
        FRotator Current = GetActorRotation();
        FRotator Target = FRotator(0.f, Current.Yaw, 0.f);
        SetActorRotation(FMath::RInterpTo(Current, Target, DeltaTime, InterpSpeed));
        return;
    }

    // 로컬 축 기준 움직임
    FQuat YawQuat = FQuat(CurrentQuat.GetAxisZ(), FMath::DegreesToRadians(LookInput.X * RotateSpeed * DeltaTime));
    FQuat PitchQuat = FQuat(CurrentQuat.GetAxisY(), FMath::DegreesToRadians(LookInput.Y * RotateSpeed * DeltaTime));
    FQuat RollQuat = FQuat(CurrentQuat.GetAxisX(), FMath::DegreesToRadians(RollInput * RotateSpeed * DeltaTime));
    SetActorRotation(YawQuat * PitchQuat * RollQuat * CurrentQuat);
}

void ADrone::HandleWingRotation(float DeltaTime)
{
    if (bGravityEnabled && MoveInput.IsNearlyZero())
        return;

    FRotator CCW = FRotator(0.f, WingRotateSpeed * DeltaTime, 0.f);
    FRotator CW = FRotator(0.f, -WingRotateSpeed * DeltaTime, 0.f);

    Wing1->AddLocalRotation(CCW);
    Wing2->AddLocalRotation(CW);
    Wing3->AddLocalRotation(CCW);
    Wing4->AddLocalRotation(CW);
}

void ADrone::Move(const FInputActionValue& Value) { MoveInput = Value.Get<FVector2D>(); }

void ADrone::MoveUp(const FInputActionValue& Value)
{
    VerticalInput = Value.Get<float>();
    if (!bGravityEnabled && VerticalInput > 0.f)
        VerticalVelocity = 0.f; // 중력 꺼진 상태에서 상승 시, 낙하 관성 제거
}

void ADrone::Look(const FInputActionValue& Value) { LookInput = Value.Get<FVector2D>(); }

void ADrone::Roll(const FInputActionValue& Value) { RollInput = Value.Get<float>(); }

void ADrone::ToggleGravity(const FInputActionValue& Value)
{
    bGravityEnabled = !bGravityEnabled;
    VerticalVelocity = 0.f;
}