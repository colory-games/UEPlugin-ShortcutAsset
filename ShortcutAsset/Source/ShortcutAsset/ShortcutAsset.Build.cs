/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

using UnrealBuildTool;

public class ShortcutAsset : ModuleRules
{
	public ShortcutAsset(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]{
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd",
		});

		PrivateDependencyModuleNames.AddRange(new string[]{
            "ApplicationCore",
			"AssetRegistry",
			"EditorStyle",
			"EditorSubsystem",
			"SlateCore",
			"Slate",
            "ToolMenus",
            "PropertyEditor",
			"ContentBrowser",
			"ContentBrowserData",
		});

		// @remove-start FULL_VERSION=true
		PublicDefinitions.Add("SA_FREE_VERSION");
		// @remove-end
	}
}