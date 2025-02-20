// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFramework/Actor.h"
#include "WeatherTransitionBase.h"
#include "Weather.h"
#include "WeatherData.h"
#include "InGameTimeController.h"
#include "NiagaraComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "WeatherController.generated.h"

UCLASS(BlueprintType, ClassGroup=(WeatherSystem), meta=(BlueprintSpawnableComponent))
class WEATHERSYSTEM_API AWeatherController : public AActor
{
	GENERATED_BODY()

	///////////////
	// Properties
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EWeather CurrentWeather;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeatherDurationMin;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeatherDurationMax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeatherChangeFrequency;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WeatherChangeFrequencyDeviation;
	float WeatherDuration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UWeatherData* WeatherData = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AInGameTimeController* IGTController = nullptr;

	UPROPERTY(EditAnywhere)
	UMaterialParameterCollection* GlobalMaterialParams = nullptr;
	UPROPERTY()
	UMaterialParameterCollectionInstance* GlobalMaterialParamsInstance = nullptr; // TODO this should probably be a TSharedPtr<>
	UPROPERTY(EditAnywhere)
	AExponentialHeightFog* AmbientFog = nullptr;
	UPROPERTY(EditAnywhere)
	ADirectionalLight* SunLight = nullptr;
	UPROPERTY(EditAnywhere)
	ADirectionalLight* MoonLight = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* Particles = nullptr;

private:
	UPROPERTY(EditAnywhere)
	float TransitionDuration = 0;
	float CurrentTransitionTime = 0;

	// TODO these should probably be TSharedPtr<>
	WeatherTransitionBase* CurrentTransition = nullptr;
	WeatherTransitionBase* PreviousTransition = nullptr;

	////////////
	// Methods
public:
	AWeatherController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	EWeather GetCurrentWeather() const { return CurrentWeather; };
	UFUNCTION(BlueprintCallable)
	void SetWeather(EWeather Weather);
	UFUNCTION(BlueprintCallable)
	void SetWeatherParameter(EWeather Weather, UDataAsset* Params) const;
	UFUNCTION(BlueprintCallable)
	void TransitionToWeather(EWeather Weather, float ElapsedTime, float Duration, bool bInverse);
	void TransitionToWeather(EWeather Weather, float ElapsedTime, float Duration);
	UFUNCTION(BlueprintCallable)
	EWeather SelectWeather() const;
	UFUNCTION(BlueprintCallable)
	static float SelectWeatherDuration(float Min, float Max);

protected:
	virtual void BeginPlay() override;

private:
	void ProcessWeatherState();
	WeatherTransitionBase* GetWeatherTransitionObject(EWeather Weather);
};
