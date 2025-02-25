#pragma once

#include "InteractionData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	// This is pretty hardcoded just to make this easily testable but the idea is that this should work as an event data
	// that you can dispatch to any interactable and they handle the event, similar to how input actions work - maybe even
	// use input action as events?
	// If you need to pass delegates (to check for a condition/predicate for example) or other references, this should
	// be an UObject instead of a struct
	
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	bool LeftMouseDown;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	bool RightMouseDown;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	FVector2D MouseAxis;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	float WheelAxis;
};
