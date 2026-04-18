#include "HumanGameMode.h"
#include "Human.h"
#include "HumanController.h"

AHumanGameMode::AHumanGameMode()
{
	DefaultPawnClass = AHumanGameMode::StaticClass();
	PlayerControllerClass = AHumanGameMode::StaticClass();
}