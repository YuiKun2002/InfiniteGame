// Copyright (C) 2020, @Author: AngrySteiner

using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using UnrealBuildTool;


public class PolygonButton : ModuleRules
{
	public PolygonButton(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

		PublicDependencyModuleNames.AddRange(new string[]
		{
			 "Core"
			,"CoreUObject"
			,"Engine"
			,"RenderCore"
			,"RHI"
			,"UMG"
			,"Slate"
			,"SlateCore"
			,"Projects"
		});

	}
}

