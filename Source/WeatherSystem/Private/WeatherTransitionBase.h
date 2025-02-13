#pragma once

#include "CoreMinimal.h"

class WEATHERSYSTEM_API WeatherTransitionBase
{
public:
	UMaterialParameterCollectionInstance* GlobalMaterialParamsInstance;
	virtual ~WeatherTransitionBase() = default;

	explicit WeatherTransitionBase(UMaterialParameterCollectionInstance* GlobalMaterialParamsInstance)
	{
		this->GlobalMaterialParamsInstance = GlobalMaterialParamsInstance;
	}

	virtual void TransitionWeather(float TransitionAlpha) = 0;
};
