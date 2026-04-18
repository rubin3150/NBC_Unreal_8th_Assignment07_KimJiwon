#include "HumanController.h"
#include "EnhancedInputSubsystems.h"

AHumanController::AHumanController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr)
{
}

void AHumanController::BeginPlay()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // EnhancedInputSystemÀ» °ü¸®ÇÏ´Â Subsystem È¹µæÇÏ±â
			if (InputMappingContext)
				Subsystem->AddMappingContext(InputMappingContext, 0);
}