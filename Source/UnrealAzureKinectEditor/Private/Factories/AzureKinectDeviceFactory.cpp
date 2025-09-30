// <copyright file="AzureKinectDeviceFactory.cpp" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2025 Visualisierungsinstitut der Universit�t Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph M�ller</author>

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

#include "AzureKinectDeviceFactory.h"

#include "AzureKinectDevice.h"
#include "AssetTypeCategories.h"


/*
 * UAzureKinectDeviceFactory::UAzureKinectDeviceFactory
 */
UAzureKinectDeviceFactory::UAzureKinectDeviceFactory(
        const FObjectInitializer& initialiser)
        : Super(initialiser) {
    this->bCreateNew = true;
    this->bEditAfterNew = true;
    this->SupportedClass = UAzureKinectDevice::StaticClass();
}


/*
 * UAzureKinectDeviceFactory::FactoryCreateNew
 */
UObject* UAzureKinectDeviceFactory::FactoryCreateNew(
        UClass *inClass,
        UObject *inParent,
        FName inName,
        EObjectFlags flags,
        UObject *context,
        FFeedbackContext *warn) {
    return NewObject<UAzureKinectDevice>(inParent, inClass, inName, flags);
}


/*
 * UAzureKinectDeviceFactory::GetMenuCategories
 */
uint32 UAzureKinectDeviceFactory::GetMenuCategories(void) const {
    return EAssetTypeCategories::Misc;
}


/*
 * UAzureKinectDeviceFactory::ShouldShowInNewMenu
 */
bool UAzureKinectDeviceFactory::ShouldShowInNewMenu(void) const {
    return true;
}


