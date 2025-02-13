// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RainData.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(WeatherSystem))
class WEATHERSYSTEM_API URainData : public UDataAsset
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> FogDensity;
	UPROPERTY(EditAnywhere)
	FColor FogColor;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> LightReductionPercentage;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> ParticleEffects;
};
