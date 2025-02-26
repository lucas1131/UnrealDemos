// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DemosEditorTarget : TargetRules
{
	public DemosEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange(new[] { "Demos" });
		ExtraModuleNames.AddRange(new[] { "WeatherSystem" });
		ExtraModuleNames.AddRange(new[] { "Interactable" });
	} 
}