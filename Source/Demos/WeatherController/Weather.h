
#pragma once

#include "Weather.generated.h"

UENUM(BlueprintType)
enum class EWeather : uint8
{
    Clear,
    Cloudy,
    Rain,
    HeavyRain,
    Snow,
    Blizzard,
    Heatwave,
    Sandstorm,

    // Transition states
    StartingRain UMETA(Hidden),
    EndingRain UMETA(Hidden),
};
