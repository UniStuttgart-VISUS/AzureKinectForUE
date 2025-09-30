// <copyright file="AzureKinectDeviceThread.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "AzureKinectDeviceThread.h"

#include "HAL/PlatformProcess.h"

#include "AzureKinectDevice.h"

DEFINE_LOG_CATEGORY(AzureKinectThreadLog);


/*
 * FAzureKinectDeviceThread::FAzureKinectDeviceThread
 */
FAzureKinectDeviceThread::FAzureKinectDeviceThread(
        UAzureKinectDevice *Device)
    : _device(Device),
        _stopCounter(0),
        _thread(nullptr) {
    this->_thread= FRunnableThread::Create(this,
        TEXT("Azure Kinect device thread"),
        0,
        TPri_BelowNormal); //windows default = 8mb for thread, could specify more

    if (!this->_thread) {
        UE_LOG(AzureKinectThreadLog,
            Error,
            TEXT("Failed to create Azure Kinect device worker thread."));
    }

}

/*
 * FAzureKinectDeviceThread::~FAzureKinectDeviceThread
 */
FAzureKinectDeviceThread::~FAzureKinectDeviceThread(void) {
    delete this->_thread;
}


/*
 * FAzureKinectDeviceThread::Init
 */
bool FAzureKinectDeviceThread::Init(void) {
    UE_LOG(AzureKinectThreadLog,
        Verbose,
        TEXT("Azure Kinect device worker thread has started."));
    return true;
}


/*
 * FAzureKinectDeviceThread::Run
 */
uint32 FAzureKinectDeviceThread::Run(void) {
    if (!this->_device) {
        UE_LOG(AzureKinectThreadLog,
            Error,
            TEXT("Kinect device is null, there is nothing to do for the ")
            TEXT("device worker thread."));
        return 1;
    }

    while (this->_stopCounter.GetValue() == 0) {
        // Do the Kinect capture, enqueue, pop body frame stuff
        this->_device->UpdateAsync();
    }

    return 0;
}


/*
 * FAzureKinectDeviceThread::Stop
 */
void FAzureKinectDeviceThread::Stop(void) {
    this->_stopCounter.Increment();
}


/*
 * FAzureKinectDeviceThread::EnsureCompletion
 */
void FAzureKinectDeviceThread::EnsureCompletion(void) {
    this->Stop();

    if (this->_thread) {
        this->_thread->WaitForCompletion();
    }
}
