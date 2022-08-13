// Basic Shooter Game. All rights reserved

using UnrealBuildTool;

public class BasicShooter : ModuleRules
{
    public BasicShooter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Niagara",
            "PhysicsCore", // Physical Materials
            "GameplayTasks",
            "NavigationSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[]
        {
            "BasicShooter/Public/AI",
            "BasicShooter/Public/AI/Tasks",
            "BasicShooter/Public/AI/Services",
            "BasicShooter/Public/AI/EQS",
            "BasicShooter/Public/AI/Decorators",
            "BasicShooter/Public/Animations",
            "BasicShooter/Public/Components",
            "BasicShooter/Public/Dev",
            "BasicShooter/Public/Menu",
            "BasicShooter/Public/Menu/UI",
            "BasicShooter/Public/Pickups",
            "BasicShooter/Public/Player",
            "BasicShooter/Public/Sound",
            "BasicShooter/Public/UI",
            "BasicShooter/Public/Weapon",
            "BasicShooter/Public/Weapon/Components",
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
