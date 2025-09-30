// <copyright file="UnrealAzureKinectEditor.Build.cs" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2025 Visualisierungsinstitut der Universit�t Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph M�ller</author>

using System.IO;
using UnrealBuildTool;


public class UnrealAzureKinectEditor : ModuleRules {

    public UnrealAzureKinectEditor(ReadOnlyTargetRules target) : base(target) {
        this.PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        this.PrivateIncludePaths.AddRange([
            "UnrealAzureKinectEditor/Private",
            "UnrealAzureKinectEditor/Private/AnimNodes",
            "UnrealAzureKinectEditor/Private/AssetTools",
            "UnrealAzureKinectEditor/Private/Customizations",
            "UnrealAzureKinectEditor/Private/Factories",
        ]);

        this.PrivateDependencyModuleNames.AddRange([
            "Core",
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "PropertyEditor",
            "InputCore",
            "UnrealAzureKinect",
            "UnrealEd",
            "BlueprintGraph",
            "AnimGraph",
        ]);
    }
}
