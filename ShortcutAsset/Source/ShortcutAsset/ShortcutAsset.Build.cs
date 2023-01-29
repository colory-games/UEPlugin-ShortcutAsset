// Some copyright should be here...

using UnrealBuildTool;

public class ShortcutAsset : ModuleRules
{
    public ShortcutAsset(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AssetRegistry",
                "EditorStyle",
                "EditorSubsystem",
                "SlateCore",
                "Slate",
                "PropertyEditor",
                "ContentBrowser",
            }
        );
    }
}