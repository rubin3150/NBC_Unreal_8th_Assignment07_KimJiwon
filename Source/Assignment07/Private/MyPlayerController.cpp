#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr)
{

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // 로컬 플레이어 정보 가져오기
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // EnhancedInputSystem을 관리하는 Subsystem 획득하기
			if (InputMappingContext) // IMC 객체 있는지 확인
				Subsystem->AddMappingContext(InputMappingContext, 0); // Subsystem에 IMC 우선순위 최우선으로 추가
}