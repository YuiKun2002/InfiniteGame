// Copyright (C) 2020, @Author: AngrySteiner

using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using UnrealBuildTool;


public class PolygonButtonEditor : ModuleRules
{
	public PolygonButtonEditor(ReadOnlyTargetRules Target) : base(Target)
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
			,"UMGEditor"
			,"UnrealEd"
			,"Projects"
			,"DetailCustomizations"
			,"PropertyEditor"
			,"PolygonButton"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			 "EditorStyle"
		});

	}
}

