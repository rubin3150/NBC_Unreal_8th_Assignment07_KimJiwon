#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HumanController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ASSIGNMENT07_API AHumanController : public APlayerController
{
	GENERATED_BODY()

public:
	AHumanController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

protected:
	virtual void BeginPlay() override;
};
