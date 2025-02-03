#include "RainTransition.h"
#include "RainData.h"
#include "Materials/MaterialParameterCollectionInstance.h"

RainTransition::RainTransition(URainData* Data, UMaterialParameterCollectionInstance* GlobalMaterialParamsInstance)
	: WeatherTransitionBase(GlobalMaterialParamsInstance)
{
	this->Data = Data;
}

void RainTransition::TransitionWeather(const float TransitionAlpha)
{
	// These should follow different timelines so they increment at different rates
	// for example, puddles should take longer to properly form compared to general wetness
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
}
