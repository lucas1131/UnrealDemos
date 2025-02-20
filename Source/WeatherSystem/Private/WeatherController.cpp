#include "WeatherController.h"
#include "RainTransition.h"

// Sets default values
AWeatherController::AWeatherController()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentWeather = EWeather::Clear;
	WeatherDurationMin = 0.0f;
	WeatherDurationMax = 0.0f;
	WeatherChangeFrequency = 0.0f;
	WeatherChangeFrequencyDeviation = 0.0f;
	WeatherDuration = -1.0;
}

void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	GlobalMaterialParamsInstance = GetWorld()->GetParameterCollectionInstance(GlobalMaterialParams);
}

void AWeatherController::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessWeatherState();
}

void AWeatherController::SetWeatherParameter(const EWeather Weather, UDataAsset* Params) const
{
	WeatherData->Params.Add(Weather, Params);
}

void AWeatherController::ProcessWeatherState()
{
}

WeatherTransitionBase* AWeatherController::GetWeatherTransitionObject(EWeather Weather)
{
	if (CurrentWeather == Weather)
	{
		return CurrentTransition;
	}

	// TODO save current transition to previous before deleting so we can blend between any two weathers
	if (PreviousTransition != nullptr)
	{
		// delete PreviousTransition;
	}
	PreviousTransition = CurrentTransition; // TODO this is leaking because its not using smart pointers yet

	switch (CurrentWeather)
	{
	case EWeather::Clear:
		// should use cached transition object with reverse alpha
		// return CurrentTransition;
		return nullptr;
	case EWeather::Rain:
	case EWeather::HeavyRain:
		CurrentTransition = new RainTransition(
			static_cast<URainData*>(WeatherData->Params[CurrentWeather]),
			GlobalMaterialParamsInstance,
			AmbientFog->GetComponent(), SunLight, MoonLight, Particles);
		return CurrentTransition;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown weather enum value %d"), (int)Weather);
		return nullptr;
	}
}

void AWeatherController::SetWeather(const EWeather Weather)
{
	TransitionToWeather(Weather, 0.0, 0.0);
}

void AWeatherController::TransitionToWeather(const EWeather Weather, const float ElapsedTime, const float Duration)
{
	TransitionToWeather(Weather, ElapsedTime, Duration, false);
}

void AWeatherController::TransitionToWeather(const EWeather Weather, const float ElapsedTime, const float Duration,
                                             const bool bInverse)
{
	// TODO Clear weather need to undo current weather somehow
	CurrentWeather = Weather;
	CurrentTransition = GetWeatherTransitionObject(Weather);
	if (CurrentTransition == nullptr)
	{
		CurrentTransition = GetWeatherTransitionObject(EWeather::Clear);
	}

	if (Duration <= 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,TEXT("Duration is 0, instantly setting new weather"));
		CurrentTransition->TransitionWeather(1.0);
	}
	else
	{
		// TODO blend with previous weather
		float TransitionAlpha = ElapsedTime / Duration;
		if (TransitionAlpha >= 1.0f)
		{
			TransitionAlpha = 1.0f;
		}

		if (bInverse)
		{
			TransitionAlpha = 1.0f - TransitionAlpha;
		}

		CurrentTransition->TransitionWeather(TransitionAlpha);
	}
}

int WeightedRandomSelect(TArray<float> Weights)
{
	int Index;
	for (Index = 1; Index < Weights.Num(); Index++)
	{
		Weights[Index] += Weights[Index - 1];
	}

	const float Random = FMath::RandRange(0, 1) * Weights[Weights.Num() - 1];

	for (Index = 0; Index < Weights.Num(); Index++)
	{
		if (Weights[Index] > Random)
		{
			break;
		}
	}

	return Index;
}

EWeather AWeatherController::SelectWeather() const
{
	TArray<float> Weights;
	WeatherData->Chance.GenerateValueArray(Weights);

	const int SelectedIndex = WeightedRandomSelect(Weights);

	TArray<EWeather> Weathers;
	WeatherData->Chance.GenerateKeyArray(Weathers);
	return Weathers[SelectedIndex];
}

float AWeatherController::SelectWeatherDuration(const float Min, const float Max)
{
	return FMath::RandRange(Min, Max);
}
