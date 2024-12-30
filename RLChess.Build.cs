// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RLChess : ModuleRules
{
	public RLChess(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", 
			"InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "MediaAssets", 
			"GeometryCollectionEngine", "Chaos", "AIModule" });

        // "GameplayTasks", "NavigationSystem", "Niagara"
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemSteam" });

    }
}
