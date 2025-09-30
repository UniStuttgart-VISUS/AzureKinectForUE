// <copyright file="UnrealAzureKinectModule.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "UnrealAzureKinect.h"

#define LOCTEXT_NAMESPACE "FUnrealAzureKinectModule"


/*
 * FUnrealAzureKinectModule::ShutdownModule
 */
void FUnrealAzureKinectModule::ShutdownModule(void) {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.

}


/*
 * FUnrealAzureKinectModule::StartupModule
 */
void FUnrealAzureKinectModule::StartupModule(void) {
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}


#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FUnrealAzureKinectModule, UnrealAzureKinect)
