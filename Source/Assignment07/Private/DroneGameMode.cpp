#include "DroneGameMode.h"
#include "Drone.h"

ADroneGameMode::ADroneGameMode()
{
    DefaultPawnClass = ADrone::StaticClass();
}