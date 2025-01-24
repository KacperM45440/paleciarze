// :P

using UnrealBuildTool;
using System.Collections.Generic;

public class PaleciarzeTarget : TargetRules
{
	public PaleciarzeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Paleciarze" } );
	}
}
