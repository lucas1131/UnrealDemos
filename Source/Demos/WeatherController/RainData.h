// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Engine/DataAsset.h"
#include "RainData.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class DEMOS_API URainData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> Wetness;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> RainIntensity;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> PuddleSize;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> WindIntensity;
};
