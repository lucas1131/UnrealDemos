#include "RainTransition.h"
#include "CoreMinimal.h"
#include "RainData.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/LightComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"

RainTransition::RainTransition(URainData* Data,
                               UMaterialParameterCollectionInstance* GlobalMaterialParamsInstance,
                               UExponentialHeightFogComponent* Fog,
                               ADirectionalLight* Sun,
                               ADirectionalLight* Moon,
                               UNiagaraComponent* Particles)
	: WeatherTransitionBase(GlobalMaterialParamsInstance)
{
	this->Data = Data;
	this->Fog = Fog;
	this->Sun = Sun;
	this->Moon = Moon;
	this->Particles = Particles;
}

void RainTransition::TransitionWeather(const float TransitionAlpha)
{
	GlobalMaterialParamsInstance->SetScalarParameterValue(
		FName("Wetness"),
		Data->Wetness->GetFloatValue(TransitionAlpha));

	GlobalMaterialParamsInstance->SetScalarParameterValue(
		FName("Rain"),
		Data->RainIntensity->GetFloatValue(TransitionAlpha));

	GlobalMaterialParamsInstance->SetScalarParameterValue(
		FName("PuddleSize"),
		Data->PuddleSize->GetFloatValue(TransitionAlpha));

	GlobalMaterialParamsInstance->SetScalarParameterValue(
		FName("Wind"),
		Data->WindIntensity->GetFloatValue(TransitionAlpha));

	if (IsValid(Fog))
	{
		Fog->FogDensity = Data->FogDensity->GetFloatValue(TransitionAlpha);
		Fog->SetFogInscatteringColor(Data->FogColor);
		Fog->Activate();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Object 'Fog' is invalid!"));
	}

	if (IsValid(Sun))
	{
		Sun->GetLightComponent()->Intensity = Data->LightReductionPercentage->GetFloatValue(TransitionAlpha);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Object 'Sun' is invalid!"));
	}

	if (IsValid(Moon))
	{
		Moon->GetLightComponent()->Intensity = Data->LightReductionPercentage->GetFloatValue(TransitionAlpha);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Object 'Moon' is invalid!"));
	}

	if (IsValid(Particles))
	{
		const float SpawnRate = Data->ParticleEffects->GetFloatValue(TransitionAlpha);
		if (SpawnRate > 0 && !Particles->IsActive())
		{
			Particles->Activate(true);
		}
		else if (SpawnRate <= 0 && Particles->IsActive())
		{
			Particles->Deactivate();
		}

		Particles->SetFloatParameter(FName("SpawnRate"), SpawnRate);
	}
}
