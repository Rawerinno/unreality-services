// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UNREALITY_SERVICES : ModuleRules
{
	public UNREALITY_SERVICES(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"UNREALITY_SERVICES",
			"UNREALITY_SERVICES/Variant_Horror",
			"UNREALITY_SERVICES/Variant_Horror/UI",
			"UNREALITY_SERVICES/Variant_Shooter",
			"UNREALITY_SERVICES/Variant_Shooter/AI",
			"UNREALITY_SERVICES/Variant_Shooter/UI",
			"UNREALITY_SERVICES/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
