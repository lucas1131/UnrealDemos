#pragma once

#include "InteractionData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	bool LeftMouseDown;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	bool RightMouseDown;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	FVector2D MouseAxis;
	UPROPERTY(BlueprintReadWrite, Category="InteractionSystem")
	float WheelAxis;
};
