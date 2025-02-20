// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DemosTarget : TargetRules
{
	public DemosTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange(new[] { "Demos" });
		ExtraModuleNames.AddRange(new[] { "WeatherSystem" });
		ExtraModuleNames.AddRange(new[] { "Interactable" });
		RegisterModulesCreatedByRider();
	}

	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[] { "DebugMenu" });
	}
}