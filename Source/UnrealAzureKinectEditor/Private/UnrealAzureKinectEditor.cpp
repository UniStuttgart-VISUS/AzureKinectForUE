// <copyright file="UnrealAzureKinectEditor.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

// Copyright 2021 Ayumu Nagamtsu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include "UnrealAzureKinectEditor.h"

#include "CoreMinimal.h"

#include "AzureKinectDevice.h"
#include "AzureKinectDeviceActions.h"
#include "AzureKinectDeviceCustomization.h"


#define LOCTEXT_NAMESPACE "FAzureKinectEditorModule"


/*
 * FUnrealAzureKinectEditorModule::ShutdownModule
 */
void FUnrealAzureKinectEditorModule::ShutdownModule(void) {
    this->UnregisterAssetTools();
    this->UnregisterCustomizations();
}


/*
 * FUnrealAzureKinectEditorModule::StartupModule
 */
void FUnrealAzureKinectEditorModule::StartupModule(void) {
    this->_deviceName = UAzureKinectDevice::StaticClass()->GetFName();
    this->RegisterAssetTools();
    this->RegisterCustomizations();
}


/*
 * FUnrealAzureKinectEditorModule::RegisterAssetTools
 */
void FUnrealAzureKinectEditorModule::RegisterAssetTools(void) {
    auto action = MakeShareable(new FAzureKinectDeviceActions());
    auto& tools = FModuleManager::LoadModuleChecked<FAssetToolsModule>(
        "AssetTools").Get();
    tools.RegisterAssetTypeActions(action);
    this->_registeredActions.Add(action);
}


/*
 * FUnrealAzureKinectEditorModule::RegisterCustomizations
 */
void FUnrealAzureKinectEditorModule::RegisterCustomizations(void) {
    auto& editor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
        "PropertyEditor");
    editor.RegisterCustomClassLayout(this->_deviceName,
        FOnGetDetailCustomizationInstance::CreateStatic(
            &FAzureKinectDeviceCustomization::MakeInstance));
}


/*
 * FUnrealAzureKinectEditorModule::UnregisterAssetTools
 */
void FUnrealAzureKinectEditorModule::UnregisterAssetTools(void) {
    auto tools = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
    if (tools != nullptr) {
        for (auto a : this->_registeredActions) {
            tools->Get().UnregisterAssetTypeActions(a);
        }
    }
}


/*
 * FUnrealAzureKinectEditorModule::UnregisterCustomizations
 */
void FUnrealAzureKinectEditorModule::UnregisterCustomizations(void) {
    auto& editor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
        "PropertyEditor");
    editor.UnregisterCustomClassLayout(this->_deviceName);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealAzureKinectEditorModule, UnrealAzureKinectEditor)
