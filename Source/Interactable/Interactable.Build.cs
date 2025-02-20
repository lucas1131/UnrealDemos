using UnrealBuildTool;

public class Interactable : ModuleRules
{
	public Interactable(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new [] { "Core", "CoreUObject", "Engine" });
	}
}