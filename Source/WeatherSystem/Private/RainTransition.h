#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "RainData.h"
#include "WeatherTransitionBase.h"
#include "Engine/DirectionalLight.h"

class WEATHERSYSTEM_API RainTransition final : public WeatherTransitionBase
{
public:
	RainTransition(URainData* Data,
	               UMaterialParameterCollectionInstance* GlobalMaterialParamsInstance,
	               UExponentialHeightFogComponent* Fog,
	               ADirectionalLight* Sun,
	               ADirectionalLight* Moon,
	               UNiagaraComponent* Particles);

	virtual void TransitionWeather(float TransitionAlpha) override;

private:
	URainData* Data;
	UExponentialHeightFogComponent* Fog;
	ADirectionalLight* Sun;
	ADirectionalLight* Moon;
	UNiagaraComponent* Particles;
};
