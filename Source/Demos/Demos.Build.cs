// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Demos : ModuleRules
{
	public Demos(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara" });

		PrivateDependencyModuleNames.AddRange(new[] { "Slate", "SlateCore" });

		PublicIncludePathModuleNames.AddRange(new[] { "Demos" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}