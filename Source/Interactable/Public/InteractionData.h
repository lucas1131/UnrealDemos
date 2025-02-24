#pragma once

#include "InteractableData.generated.h"

USTRUCT(BlueprintType)
struct FInteractableData
{
	GENERATED_BODY()

public:
	bool LeftMouseDown;
	bool RightMouseDown;
	FVector2D MouseMovement;
};
