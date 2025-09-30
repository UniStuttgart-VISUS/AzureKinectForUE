// <copyright file="AzureKinectDeviceActions.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "AzureKinectDeviceActions.h"

#include "AzureKinectDevice.h"


/*
 * FAzureKinectDeviceActions::GetCategories
 */
uint32 FAzureKinectDeviceActions::GetCategories(void) noexcept {
    return EAssetTypeCategories::Media;
}


/*
 * FAzureKinectDeviceActions::GetName
 */
FText FAzureKinectDeviceActions::GetName(void) const {
    return NSLOCTEXT("AzureKinectDeviceActions",
        "AssetTypeActions_AzureKinectDevice",
        "Azure Kinect Device");
}


/*
 * FAzureKinectDeviceActions::GetSupportedClass
 */
UClass *FAzureKinectDeviceActions::GetSupportedClass(void) const {
    return UAzureKinectDevice::StaticClass();
}


/*
 * FAzureKinectDeviceActions::GetTypeColor
 */
FColor FAzureKinectDeviceActions::GetTypeColor(void) const {
    return FColor::White;
}
