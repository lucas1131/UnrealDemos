#pragma once

#include "InteractionData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	// This is pretty hardcoded just to make this easily testable but the idea is that this should work as an event data
	// that you can dispatch to any interactable and they handle the event, similar to how input actions work - maybe even
	// use input action as events?
	
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	bool LeftMouseDown;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	bool RightMouseDown;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	FVector2D MouseAxis;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	float WheelAxis;
};
