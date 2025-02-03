
#pragma once

#include "TimeOfDay.generated.h"

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
    None UMETA(Hidden),
    Dawn,
    Day,
    Dusk,
    Night
};