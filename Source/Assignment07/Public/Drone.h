#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class ASSIGNMENT07_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	ADrone();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Wing1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Wing2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Wing3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Wing4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

protected:
	FVector2D MoveInput;
	FVector2D LookInput;
	float VerticalInput;
	float RollInput;
	float VerticalVelocity;
	bool bGravityEnabled;
	bool bIsGrounded;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotateSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SpeedFactor;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Gravity;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float GroundTraceDistance;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WingRotateSpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleMovement(float DeltaTime);
	void HandleRotation(float DeltaTime);
	void HandleGravity(float DeltaTime, FVector& Delta);
	void HandleWingRotation(float DeltaTime);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void MoveUp(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Roll(const FInputActionValue& Value);

	UFUNCTION()
	void ToggleGravity(const FInputActionValue& Value);
};