// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TDS : ModuleRules
{
	public TDS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
        PublicIncludePaths.AddRange(new string[]{"TDS/Components", "TDS/Character","TDS/Items","TDS/DamageType", "TDS/StateEffects"});
	}
}
