// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weather.h"
#include "WeatherData.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class DEMOS_API UWeatherData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<EWeather, float> Chance;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<EWeather, UDataAsset *> Params;
};
