// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RLChess : ModuleRules
{
	public RLChess(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "Slate", "SlateCore", "AIModule", "GameplayTasks", "NavigationSystem", "Niagara", "MediaAssets" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateDependencyModuleNames.Add("OnlineSubsystem");

    }
}
