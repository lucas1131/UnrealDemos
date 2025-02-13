#pragma once

#include "Weather.generated.h"

UENUM(BlueprintType)
enum class EWeather : uint8
{
	Clear,
	Rain,
	HeavyRain,
};
