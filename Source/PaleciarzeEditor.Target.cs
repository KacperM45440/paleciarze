// :P

using UnrealBuildTool;
using System.Collections.Generic;

public class PaleciarzeEditorTarget : TargetRules
{
	public PaleciarzeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Paleciarze" } );
	}
}
