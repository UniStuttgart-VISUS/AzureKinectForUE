// <copyright file="UnrealAzureKinect.Build.cs" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

using Microsoft.Extensions.Logging;
using System;
using System.IO;
using System.Linq;
using UnrealBuildTool;


/// <summary>
/// Configures the UnrealAzureKinect module.
/// </summary>
public class UnrealAzureKinect : ModuleRules {

    public const string SdkEnvironmentVariable = "AZUREKINECT_SDK";
    public const string BodySdkEnvironmentVariable = "AZUREKINECT_BODY_SDK";

    public UnrealAzureKinect(ReadOnlyTargetRules target) : base(target) {
        this.PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        var sdkPath = Environment.GetEnvironmentVariable(SdkEnvironmentVariable);
        if (!Directory.Exists(sdkPath)) {
            sdkPath = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
            sdkPath = Path.Combine(sdkPath, "Azure Kinect SDK v1.4.1");
            this.Logger.LogWarning("Could not derive Azure Kinect SDK from "
                + "environment variable {EnvVar}. Trying well-known location "
                + "\"{SdkPath}\" instead.", SdkEnvironmentVariable, sdkPath);
        }

        if (!Directory.Exists(sdkPath)) {
            this.Logger.LogError("Azure Kinect SDK was not found. Please set "
                + "the {EnvVar} environment variable to point to the "
                + "installation directory of the Azure Kinect SDK.",
                SdkEnvironmentVariable);
        }

        var bodySdkPath = Environment.GetEnvironmentVariable(BodySdkEnvironmentVariable);
        if (!Directory.Exists(bodySdkPath)) {
            bodySdkPath = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
            bodySdkPath = Path.Combine(bodySdkPath, "Azure Kinect Body Tracking SDK");
            this.Logger.LogWarning("Could not derive body SDK from environment "
                + "variable {EnvVar}. Trying well-known location \"{SdkPath}\" "
                + "instead.", BodySdkEnvironmentVariable, bodySdkPath);
        }

        if (!Directory.Exists(bodySdkPath)) {
            this.Logger.LogError("Azure Kinect Body Tracking SDK was not "
                + "found. Please set the {EnvVar} environment variable to "
                + "point to the installation directory of the Azure Kinect "
                + "Body Tracking SDK.", BodySdkEnvironmentVariable);
        }

        this.PublicIncludePaths.AddRange([
            Path.Combine(sdkPath, "sdk", "include"),
            Path.Combine(bodySdkPath, "sdk", "include")
        ]);

        this.PublicAdditionalLibraries.AddRange([
            Path.Combine(sdkPath, "sdk", "windows-desktop", "amd64", "release", "lib", "k4a.lib"),
            Path.Combine(sdkPath, "sdk", "windows-desktop", "amd64", "release", "lib", "k4arecord.lib"),
            Path.Combine(bodySdkPath, "sdk", "windows-desktop", "amd64", "release", "lib", "k4abt.lib")
        ]);

        var sdkBinaryDir = Path.Combine(sdkPath, "sdk", "windows-desktop", "amd64", "release", "bin");
        var sdkBinaries = Directory.GetFiles(sdkBinaryDir, "*.dll");
        var bodySdkBinaryDir = Path.Combine(bodySdkPath, "sdk", "windows-desktop", "amd64", "release", "bin");
        var bodySdkBinaries = Directory.GetFiles(bodySdkBinaryDir, "*.dll");
        var onnxModels = Directory.GetFiles(bodySdkBinaryDir, "*.onnx");
        // A4KBT depends on cudnn and several other CUDA libraries. It will not
        // start even in DirectML mode if these libraries are not found (the
        // library explicitly checks for them and fails to intialise).
        // Therefore, we copy them from the Body SDK tools such that we do not
        // have to redistribute them ourselves. Note that it is not sufficient
        // to copy the DLLs to the plugin directory, but we must also crowbar
        // it into the general search path for DLLs as the SDK is completely
        // unaware of what UE does.
        var bodySdkToolsDir = Path.Combine(bodySdkPath, "tools");
        var bodySdkToolDependencies = Directory.GetFiles(bodySdkToolsDir, "*.dll");

        var binaries = sdkBinaries
            .Concat(bodySdkBinaries)
            .Concat(bodySdkToolDependencies)
            .DistinctBy(Path.GetFileName);

        foreach (var d in binaries) {
            System.Diagnostics.Debug.Assert(File.Exists(d));
            var f = Path.GetFileName(d);
            this.RuntimeDependencies.Add($"$(BinaryOutputDir)/{f}", d);
            this.PublicDelayLoadDLLs.Add(d);
        }

        foreach (var d in onnxModels) {
            System.Diagnostics.Debug.Assert(File.Exists(d));
            var f = Path.GetFileName(d);
            this.RuntimeDependencies.Add($"$(BinaryOutputDir)/{f}", d, StagedFileType.NonUFS);
        }

        this.PrivateIncludePaths.AddRange([ "UnrealAzureKinect/Private" ]);

        this.PrivateDependencyModuleNames.AddRange([
            "Core",
            "CoreUObject",
            "Engine",
            "RenderCore",
            "RHI",
            "AnimGraphRuntime"
        ]);
    }
}
