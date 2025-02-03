// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameTimeController.h"
#include "TimeOfDay.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
AInGameTimeController::AInGameTimeController()
{
	Setup(0.0, false);
}

// Called when the game starts or when spawned
void AInGameTimeController::BeginPlay()
{
	Setup(CurrentTimeSeconds, false);
	UpdateTimeOfDayTransitions();
	Super::BeginPlay();
}

void AInGameTimeController::Setup(const float StartTime, const bool bPaused)
{
	IsPaused = bPaused;
	PrimaryActorTick.bCanEverTick = !IsPaused;
	CurrentTimeSeconds = StartTime;
	TimeOfDay = ETimeOfDay::None;

	SunriseDurationSeconds = MorningTimeSeconds - SunriseTimeSeconds;
	SunsetDurationSeconds = NightTimeSeconds - SunsetTimeSeconds;

	if (SunLight == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("Null reference to SunLight actor. Did you forget to assign the reference in the editor?"));
	}
	else
	{
		SunLight->SetLightColor(DayLightColor);
	}

	if (MoonLight == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("Null reference to MoonLight actor. Did you forget to assign the reference in the editor?"));
	}
	else
	{
		MoonLight->SetLightColor(NightLightColor);
	}

	if (SkyDome == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("Null reference to SkyDome actor. Did you forget to assign the reference in the editor?"));
	}
	else
	{
		UStaticMeshComponent* Mesh = SkyDome->GetStaticMeshComponent();
		UMaterialInterface* Material = Mesh->GetMaterial(0);
		SkyDomeMaterial = UMaterialInstanceDynamic::Create(Material, this);
		Mesh->SetMaterial(0, SkyDomeMaterial);
	}
}

void AInGameTimeController::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsPaused) return;
	
	// current time [0, 24) h
	// current time [0, 1440) m
	// current time [0, 86400) s
	// angle / s = 0.00416666666
	CurrentTimeSeconds += DeltaTime * CycleSpeed;
	UpdateDayNightCycle();
}

void AInGameTimeController::PauseTime(const bool bPause)
{
	PrimaryActorTick.bCanEverTick = !bPause;
	IsPaused = bPause;
}

void AInGameTimeController::SetTime(const float TimeSeconds)
{
	CurrentTimeSeconds = TimeSeconds;
	UpdateDayNightCycle();
}

void AInGameTimeController::UpdateDayNightCycle()
{
	/* I want to correct sun/moon speed so that their speed is slower during the day (longer days) and faster at night
	 * The sun is at angle 0 at 6h and I want to correct it based on the defined SunriseTime and NightTime as the exact
	 * moments the sun is starting to appear or disappear at the horizon.
	 * 
	 * Normal time = angle relation
	 * 6h = 180° (or maybe 0, IDK, need to check in-engine, but it shouldn't really matter, just add an offset later)
	 * 12h = 270°
	 * 18h = 0°
	 * 0h = 90°
	 */

	const float DayDuration = NightTimeSeconds - SunriseTimeSeconds;
	const float NightDuration = TotalDayTime - NightTimeSeconds + SunriseTimeSeconds; // Time wraps to 0 at midnight, be careful
	
	float ModifiedAngle;
	if (CurrentTimeSeconds > NightTimeSeconds)
	{
		// Between sunset end and midnight
		ModifiedAngle = FMath::Lerp(0.0, 90.0, (CurrentTimeSeconds - NightTimeSeconds)/(NightDuration/2.0));
	}
	else if (CurrentTimeSeconds > SunriseTimeSeconds)
	{
		// Between sunrise start sunset end
		ModifiedAngle = FMath::Lerp(180.0, 360.0, (CurrentTimeSeconds - SunriseTimeSeconds)/DayDuration);
	}
	else
	{
		ModifiedAngle = FMath::Lerp(90, 180.0, CurrentTimeSeconds/(NightDuration/2.0));
	}
	
	UpdateSunPosition(ModifiedAngle);
	UpdateMoonPosition(ModifiedAngle);
	UpdateTimeOfDayTransitions();
}

void AInGameTimeController::UpdateSunPosition(const float Angle) const
{
	const FRotator Rotator = {Angle, 180.0, 180.0};
	SunLight->SetActorRotation(Rotator);
}

void AInGameTimeController::UpdateMoonPosition(const float Angle) const
{
	const FRotator Rotator = {Angle+180, 180.0, 180.0};
	MoonLight->SetActorRotation(Rotator);
}

void AInGameTimeController::UpdateTimeOfDayTransitions()
{
	if (CurrentTimeSeconds >= SunriseTimeSeconds && CurrentTimeSeconds < MorningTimeSeconds)
	{
		TimeOfDay = ETimeOfDay::Dawn;
		UpdateSunriseTransition();
	}
	else if (CurrentTimeSeconds >= MorningTimeSeconds && CurrentTimeSeconds < SunsetTimeSeconds)
	{
		if (TimeOfDay != ETimeOfDay::Day)
		{
			TimeOfDay = ETimeOfDay::Day;
			SetDayNightTransitionValue(0.0);
		}
	}
	else if (CurrentTimeSeconds >= SunsetTimeSeconds && CurrentTimeSeconds < NightTimeSeconds)
	{
		TimeOfDay = ETimeOfDay::Dusk;
		UpdateSunsetTransition();
	}
	else if (CurrentTimeSeconds >= NightTimeSeconds || CurrentTimeSeconds < SunriseTimeSeconds)
	{
		if (TimeOfDay != ETimeOfDay::Night)
		{
			TimeOfDay = ETimeOfDay::Night;
			SetDayNightTransitionValue(1.0);
		}
	}

	if (CurrentTimeSeconds >= TotalDayTime)
	{
		CurrentTimeSeconds = 0;
	}
}

void AInGameTimeController::UpdateSunriseTransition() const
{
	const float SunriseElapsedTime = CurrentTimeSeconds - SunriseTimeSeconds;
	const float Transition = FMath::Clamp(SunriseElapsedTime / SunriseDurationSeconds, 0.0, 1.0);

	SetDayNightTransitionValue(1 - Transition);
}

void AInGameTimeController::UpdateSunsetTransition() const
{
	const float SunsetElapsedTime = CurrentTimeSeconds - SunsetTimeSeconds;
	const float Transition = FMath::Clamp(SunsetElapsedTime / SunsetDurationSeconds, 0.0, 1.0);

	SetDayNightTransitionValue(Transition);
}

void AInGameTimeController::SetDayNightTransitionValue(const float Alpha) const
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		5.0f,
		FColor::Red,
		FString::Printf(TEXT("Skybox transition alpha: %f"), Alpha));
	checkf(SkyDomeMaterial != nullptr, TEXT("SkyDome is null"));
	SkyDomeMaterial->SetScalarParameterValue(FName("DayNightTransition"), Alpha);

}

#if WITH_EDITOR
void AInGameTimeController::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	UE_LOG(LogTemp, Warning, TEXT("Post Edit Change"));

	Super::PostEditChangeProperty(e);

	const FName PropertyName = (e.Property != nullptr) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AInGameTimeController, CurrentTimeSeconds))
	{
		UpdateDayNightCycle();
	}
}
#endif
