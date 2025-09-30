// <copyright file="AzureKinectDeviceCustomization.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "AzureKinectDeviceCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"

#include "Widgets/Input/SComboBox.h"

#define LOCTEXT_NAMESPACE "AzureKinectDeviceCustomization"


/*
 * FAzureKinectDeviceCustomization::MakeInstance
 */
TSharedRef<IDetailCustomization>
FAzureKinectDeviceCustomization::MakeInstance(void) {
    return MakeShareable(new FAzureKinectDeviceCustomization());
}


/*
 * FAzureKinectDeviceCustomization::CustomizeDetails
 */
void FAzureKinectDeviceCustomization::CustomizeDetails(IDetailLayoutBuilder& builder) {
    TArray<TWeakObjectPtr<UObject>> objects;
    builder.GetObjectsBeingCustomized(objects);

    if (objects.Num() == 1) {
        this->_device = Cast<UAzureKinectDevice>(objects[0].Get());

        auto &catConfig = builder.EditCategory("Config");
        auto &catIO = builder.EditCategory("I/O");
        auto deviceOpen = TAttribute<bool>::Create(
            TAttribute<bool>::FGetter::CreateLambda([this](void) {
            return this->_device->IsOpen();
        }));

        this->_selection = this->_device->Devices[0];

        catConfig.AddCustomRow(LOCTEXT("DeviceSelectionFilterString", "Device Selection"))
            .NameContent()
            [
                SNew(STextBlock).Text(LOCTEXT("DeviceSelectionLabel", "Device Selection"))
            ]
            .ValueContent()
            [
                SNew(SComboBox<TSharedPtr<FString>>)
                    .IsEnabled(deviceOpen)
                    .OptionsSource(&(this->_device->Devices))
                    .OnSelectionChanged_Raw(this, &FAzureKinectDeviceCustomization::OnSelectionChanged)
                    .OnGenerateWidget_Raw(this, &FAzureKinectDeviceCustomization::MakeWidgetForOption)
                    .InitiallySelectedItem(this->_selection)
                    [
                        SNew(STextBlock).Text(this, &FAzureKinectDeviceCustomization::GetSelectionLabel)
                    ]
            ];

        auto DepthMode = builder.GetProperty(GET_MEMBER_NAME_CHECKED(UAzureKinectDevice, DepthMode));
        auto ColorMode = builder.GetProperty(GET_MEMBER_NAME_CHECKED(UAzureKinectDevice, ColourResolution));
        auto Fps = builder.GetProperty(GET_MEMBER_NAME_CHECKED(UAzureKinectDevice, FrameRate));
        auto SensorOrientation = builder.GetProperty(GET_MEMBER_NAME_CHECKED(UAzureKinectDevice, SensorOrientation));
        auto RemapMode = builder.GetProperty(GET_MEMBER_NAME_CHECKED(UAzureKinectDevice, Remapping));
        auto SkeletonTracking = builder.GetProperty(GET_MEMBER_NAME_CHECKED(UAzureKinectDevice, SkeletonTracking));

        catConfig.AddProperty(DepthMode)
            .IsEnabled(deviceOpen);

        catConfig.AddProperty(ColorMode)
            .IsEnabled(deviceOpen);

        catConfig.AddProperty(Fps)
            .IsEnabled(deviceOpen);

        catConfig.AddProperty(SensorOrientation)
            .IsEnabled(deviceOpen);

        catConfig.AddProperty(RemapMode)
            .IsEnabled(deviceOpen);

        catConfig.AddProperty(SkeletonTracking)
            .IsEnabled(deviceOpen);

        catIO.AddCustomRow(LOCTEXT("ButtonFilterString", "Function Buttons"))
            .NameContent()
            [
                SNew(STextBlock).Text(LOCTEXT("ExecutionLabel", "Execution"))
            ]
            .ValueContent()
            [
                SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .Padding(FMargin(0.f, 2.f, 10.f, 2.f))
                    .AutoWidth()
                    [
                        SNew(SButton)
                            .Text(LOCTEXT("LoadButtonText", "Refresh"))
                            .Visibility_Lambda([this](void) { return this->_device->IsOpen() ? EVisibility::Collapsed : EVisibility::Visible; })
                            .OnClicked_Lambda([this](void) { this->_device->RefreshDevices(); return FReply::Handled(); })
                    ]
                    + SHorizontalBox::Slot()
                    .Padding(FMargin(0.f, 2.f, 10.0f, 2.f))
                    .AutoWidth()
                    [
                        SNew(SButton)
                            .Text(LOCTEXT("StartButtonText", "Start"))
                            .Visibility_Lambda([this](void) { return this->_device->IsOpen() ? EVisibility::Collapsed : EVisibility::Visible; })
                            .OnClicked_Lambda([this](void) { this->_device->Start(); return FReply::Handled(); })
                    ]
                    + SHorizontalBox::Slot()
                    .Padding(FMargin(0.f, 0, 10.f, 2.f))
                    .AutoWidth()
                    [
                        SNew(SButton)
                            .Text(LOCTEXT("StopButtonText", "Stop"))
                            .Visibility_Lambda([this](void) { return this->_device->IsOpen() ? EVisibility::Visible : EVisibility::Collapsed; })
                            .OnClicked_Lambda([this](void) { this->_device->Stop(); return FReply::Handled(); })
                    ]
            ];
    }
}


/*
 * FAzureKinectDeviceCustomization::GetSelectionLabel
 */
FText FAzureKinectDeviceCustomization::GetSelectionLabel(void) const {
    return (this->_selection.IsValid())
        ? FText::FromString(*this->_selection)
        : LOCTEXT("InvalidAzureKinectDevice", "No Device");
}


/*
 * FAzureKinectDeviceCustomization::MakeWidgetForOption
 */
TSharedRef<SWidget> FAzureKinectDeviceCustomization::MakeWidgetForOption(
        TSharedPtr<FString> option) {
    return SNew(STextBlock).Text(FText::FromString(*option));
}


/*
 * FAzureKinectDeviceCustomization::OnSelectionChanged
 */
void FAzureKinectDeviceCustomization::OnSelectionChanged(
        TSharedPtr<FString> selection,
        ESelectInfo::Type) {
    this->_selection = selection;

    // Also update UAzureKinectDevice's current index
    this->_device->DeviceIndex = this->_device->Devices.Find(this->_selection);
}

#undef LOCTEXT_NAMESPACE
