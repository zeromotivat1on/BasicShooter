// Basic Shooter Game. All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class BasicShooterEditorTarget : TargetRules
{
	public BasicShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "BasicShooter" } );
	}
}
