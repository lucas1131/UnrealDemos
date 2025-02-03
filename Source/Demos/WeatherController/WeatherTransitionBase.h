#pragma once

#include "CoreMinimal.h"

class DEMOS_API WeatherTransitionBase
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
