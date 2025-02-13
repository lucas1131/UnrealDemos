// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "TimeOfDay.h"
#include "InGameTimeController.generated.h"

UCLASS(ClassGroup=(WeatherSystem), meta=(BlueprintSpawnableComponent))
class WEATHERSYSTEM_API AInGameTimeController : public AActor
{
	GENERATED_BODY()

	///////////////
	// Properties
protected:
	UPROPERTY(BlueprintReadWrite)
	bool IsPaused;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CurrentTimeSeconds;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ETimeOfDay TimeOfDay;

private:
	float TickDeltaTime;

	UPROPERTY(EditAnywhere)
	ADirectionalLight* SunLight;
	UPROPERTY(EditAnywhere)
	ADirectionalLight* MoonLight;

	UPROPERTY(EditAnywhere)
	AStaticMeshActor* SkyDome;
	UPROPERTY()
	UMaterialInstanceDynamic* SkyDomeMaterial;

	// CycleSpeed = 1 means realtime (1s IRL == 1ms in game). By default, speed is 60 times faster so 1 second IRL == 1 minute in game.
	UPROPERTY(EditAnywhere)
	float CycleSpeed = 60.0;
	UPROPERTY(EditAnywhere)
	float SunriseTimeSeconds = 14400; // 4h
	UPROPERTY(EditAnywhere)
	float MorningTimeSeconds = 21600; // 6h
	UPROPERTY(EditAnywhere)
	float SunsetTimeSeconds = 66600; // 18h30m
	UPROPERTY(EditAnywhere)
	float NightTimeSeconds = 72000; // 20h

	const float TotalDayTime = 86400.0 - 1.0;
	float SunriseDurationSeconds;
	float SunsetDurationSeconds;

	UPROPERTY(EditAnywhere)
	FColor DayLightColor = FColor::White;
	UPROPERTY(EditAnywhere)
	FColor NightLightColor = FColor::White;

	////////////
	// Methods
public:
	AInGameTimeController();
	void Setup(float StartTime, bool bPaused);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetTime(float TimeSeconds);
	UFUNCTION(BlueprintCallable)
	void PauseTime(bool bPause);

	UFUNCTION(BlueprintCallable)
	float GetCurrentTime() const { return CurrentTimeSeconds; }

	UFUNCTION(BlueprintCallable)
	float GetInGameDeltaTime() const { return CycleSpeed * TickDeltaTime * !IsPaused; }

	// If paused, delta time should always be 0

	UFUNCTION(BlueprintCallable)
	bool IsTimePaused() const
	{
		return IsPaused;
	}

	UFUNCTION(BlueprintCallable)
	float RealTimeToInGameTimeDurationSeconds(const float Duration) const { return Duration * CycleSpeed; }

	UFUNCTION(BlueprintCallable)
	float InGameTimeToRealTimeDurationSeconds(const float Duration) const { return Duration / CycleSpeed; }

protected:
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable)
	void UpdateDayNightCycle();

private:
	void UpdateSunPosition(float Angle) const;
	void UpdateMoonPosition(float Angle) const;
	void UpdateTimeOfDayTransitions();
	void UpdateSunriseTransition() const;
	void SetDayNightTransitionValue(const float Alpha) const;
	void UpdateSunsetTransition() const;
};
