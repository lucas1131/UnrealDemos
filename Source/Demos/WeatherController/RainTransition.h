#pragma once

#include "CoreMinimal.h"
#include "RainData.h"
#include "WeatherTransitionBase.h"

class DEMOS_API RainTransition final : public WeatherTransitionBase
{
public:
	RainTransition(URainData* Data, UMaterialParameterCollectionInstance* GlobalMaterialParamsInstance);

	virtual void TransitionWeather(float TransitionAlpha) override;

private:
	URainData* Data;
};
