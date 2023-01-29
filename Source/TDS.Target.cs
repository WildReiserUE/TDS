// Created WildReiser ©2022

using UnrealBuildTool;
using System.Collections.Generic;

public class TDSTarget : TargetRules
{
	public TDSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("TDS");
	}
}
