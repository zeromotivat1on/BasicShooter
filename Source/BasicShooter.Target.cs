// Basic Shooter Game. All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class BasicShooterTarget : TargetRules
{
	public BasicShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "BasicShooter" } );
	}
}
