// <copyright file="AzureKinectDevice.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "AzureKinectDevice.h"

#include <cassert>

#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"

#include "Runtime/RHI/Public/RHI.h"

#include "AzureKinectDeviceThread.h"


DEFINE_LOG_CATEGORY(AzureKinectDeviceLog);


/*
 * UAzureKinectDevice::CountDevices
 */
int32 UAzureKinectDevice::CountDevices(void) {
    return k4a_device_get_installed_count();
}


/*
 * UAzureKinectDevice::UAzureKinectDevice
 */
UAzureKinectDevice::UAzureKinectDevice(void)
        : BodyIndexTexture(nullptr),
        ColourResolution(EKinectColourResolution::RESOLUTION_720P),
        ColourTexture(nullptr),
        DepthMode(EKinectDepthMode::NFOV_UNBINNED),
        DepthTexture(nullptr),
        DeviceIndex(-1),
        DisableStreamingIndicator(false),
        FrameRate(EKinectFps::PER_SECOND_30),
        InfraredTexture(nullptr),
        Remapping(EKinectRemap::DEPTH_TO_COLOUR),
        SensorOrientation(EKinectSensorOrientation::DEFAULT),
        SkeletonTracking(EKinectTrackerProcessing::DISABLED),
        SynchronisedImagesOnly(false),
        _cntTrackedSkeletons(0),
        _thread(nullptr) {
    this->RefreshDevices();
}


/*
 * UAzureKinectDevice::UAzureKinectDevice
 */
UAzureKinectDevice::UAzureKinectDevice(const FObjectInitializer& initialiser)
        : Super(initialiser),
        _cntTrackedSkeletons(0),
        _thread(nullptr) {
    this->RefreshDevices();
}


/*
 * UAzureKinectDevice::GetSkeletons
 */
TArray<FAzureKinectSkeleton> UAzureKinectDevice::GetSkeletons(void) const {
    if (this->IsOpen()) {
        FScopeLock l(&this->_lock);
        return this->_skeletons;
    } else {
        return this->_skeletons;
    }
}


/*
 * UAzureKinectDevice::GetSkeleton
 */
FAzureKinectSkeleton UAzureKinectDevice::GetSkeleton(const int32 index) const {
    if (!this->IsOpen()) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("An empty skeleton was returned as the Kinect device is ")
            TEXT("not open."));
        return FAzureKinectSkeleton();
    }

    if (this->SkeletonTracking == EKinectTrackerProcessing::DISABLED) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("An empty skeleton was returned as skeleton tracking is ")
            TEXT("disabled."));
        return FAzureKinectSkeleton();
    }

    FScopeLock l(&this->_lock);
    if (!this->_skeletons.IsValidIndex(index)) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("An empty skeleton was returned as the requested index %d ")
            TEXT("is out of range."), index);
        return FAzureKinectSkeleton();
    }

    return this->_skeletons[index];
}


/*
 * UAzureKinectDevice::GetTrackedSkeletons
 */
int32 UAzureKinectDevice::GetTrackedSkeletons(void) const {
    if (!this->IsOpen()) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("No skeletons are tracked as the device is not open."));
        return 0;
    }

    if (this->SkeletonTracking == EKinectTrackerProcessing::DISABLED) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("No skeletons are tracked as tracking is disabled."));
        return 0;
    }

    FScopeLock l(&this->_lock);
    return this->_cntTrackedSkeletons;
}


/*
 * UAzureKinectDevice::RefreshDevices
 */
int32 UAzureKinectDevice::RefreshDevices(void) {
    const auto retval = CountDevices();

    this->Devices.Empty(retval);

    for (int32 i = 0; i < retval; ++i) {
        try {
            auto d = k4a::device::open(i);
            this->Devices.Add(MakeShared<FString>(d.get_serialnum().c_str()));
            FString serial(ANSI_TO_TCHAR(d.get_serialnum().c_str()));
            UE_LOG(AzureKinectDeviceLog,
                Verbose,
                TEXT("Found Azure Kinect %s."),
                *serial);
        } catch (k4a::error ex) {
            FString msg(ANSI_TO_TCHAR(ex.what()));
            UE_LOG(AzureKinectDeviceLog,
                Error,
                TEXT("Failed opening device %d: %s"),
                i, *msg);
        }
    }

    return this->Devices.Num();
}

/*
 * UAzureKinectDevice::Start
 */
bool UAzureKinectDevice::Start(void) {
    if (this->IsOpen()) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("The Azure Kinect device is already open."));
        return false;
    }

    if (this->DeviceIndex < 0) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("No Azure Kinect has been selected. Make sure to set the ")
            TEXT("device index before starting the device."));
        return false;
    }

    try {
        this->_device = k4a::device::open(this->DeviceIndex);

        {
            auto config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
            typedef decltype(config.camera_fps) F;
            typedef decltype(config.color_resolution) R;
            typedef decltype(config.depth_mode) D;
            config.camera_fps = static_cast<F>(this->FrameRate);
            config.color_format = ColourFormat;
            config.color_resolution = static_cast<R>(this->ColourResolution);
            config.depth_mode = static_cast<D>(this->DepthMode);
            config.disable_streaming_indicator
                = this->DisableStreamingIndicator;
            config.synchronized_images_only = this->SynchronisedImagesOnly;
            config.wired_sync_mode = K4A_WIRED_SYNC_MODE_STANDALONE;

            this->_device.start_cameras(&config);
            this->_calibration = this->_device.get_calibration(config.depth_mode,
                config.color_resolution);
            this->_transform = k4a::transformation(this->_calibration);
        }

        if (this->SkeletonTracking != EKinectTrackerProcessing::DISABLED) {
            auto config = K4ABT_TRACKER_CONFIG_DEFAULT;
            typedef decltype(config.sensor_orientation) O;
            typedef decltype(config.processing_mode) P;
            config.sensor_orientation = static_cast<O>(this->SensorOrientation);
            config.processing_mode = static_cast<P>(this->SkeletonTracking);

            // Ensure that k4abt.dll looks in our plugin folder for
            // dependencies.
            auto prevPath = FPlatformMisc::GetEnvironmentVariable(TEXT("PATH"));
            auto myPath = GetPluginLocation();
            auto newPath = prevPath + TEXT(";") + myPath;
            FPlatformMisc::SetEnvironmentVar(TEXT("PATH"), *newPath);

            this->_bodyTracker = k4abt::tracker::create(this->_calibration,
                config);
        }

        this->_frameTime = ToFrameTime(this->FrameRate);

        assert(this->_thread == nullptr);
        this->_thread = new FAzureKinectDeviceThread(this);
    } catch (k4a::error ex) {
        if (this->_device) {
            this->_device.close();
        }

        FString msg(ANSI_TO_TCHAR(ex.what()));
        UE_LOG(AzureKinectDeviceLog,
            Error,
            TEXT("Failed starting device %d: %s"), this->DeviceIndex, *msg);
        return false;
    }

    return true;
}


/*
 * UAzureKinectDevice::Stop
 */
bool UAzureKinectDevice::Stop(void) {
    if (!this->IsOpen()) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("Azure Kinect is not running."));
        return false;
    }

    if (this->_thread != nullptr) {
        this->_thread->EnsureCompletion();
        this->_thread = nullptr;
    }

    if (this->_bodyTracker) {
        this->_bodyTracker.shutdown();
        this->_bodyTracker.destroy();
    }

    if (this->_remapImage) {
        this->_remapImage.reset();
    }

    if (this->_device) {
        this->_device.stop_cameras();
        this->_device.close();
        UE_LOG(AzureKinectDeviceLog,
            Verbose,
            TEXT("Azure Kinect %d was closed."), this->DeviceIndex);
    }

    return !this->_device;
}


/*
 * UAzureKinectDevice::GetPluginLocation
 */
FString UAzureKinectDevice::GetPluginLocation(void) {
    constexpr auto FLAGS = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
        | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;
    HMODULE hModule = NULL;
    FString retval;
    retval.GetCharArray().SetNumUninitialized(MAX_PATH);

    if (::GetModuleHandleEx(FLAGS,
            // Note: any function or variable in the plugin DLL would do.
            reinterpret_cast<LPCTSTR>(&GetPluginLocation),
            &hModule)) {
        auto length = ::GetModuleFileName(hModule,
            retval.GetCharArray().GetData(),
            retval.GetAllocatedSize());
        if ((length > 0) && (length < retval.GetAllocatedSize())) {
            retval.GetCharArray().SetNum(length);
            retval = FPaths::GetPath(retval);
        } else {
            retval.Empty();
        }
    }

    return retval;
}


/*
 * UAzureKinectDevice::ToFrameTime
 */
std::chrono::milliseconds UAzureKinectDevice::ToFrameTime(
        const EKinectFps frameRate) noexcept {
    auto value = 0.0f;

    switch (frameRate) {
        case EKinectFps::PER_SECOND_5:
            value = 1000.f / 5.f;
            break;

        case EKinectFps::PER_SECOND_15:
            value = 1000.f / 15.f;
            break;

        case EKinectFps::PER_SECOND_30:
            value = 1000.f / 30.f;
            break;
    }

    return std::chrono::milliseconds(FMath::CeilToInt(value));
}


/*
 * UAzureKinectDevice::ToTransform
 */
FTransform UAzureKinectDevice::ToTransform(const k4abt_joint_t& joint,
        const int32 index) {
    // This transform algorithm is introdeced from 
    // https://github.com/secretlocation/azure-kinect-unreal/
    // Still there is room to refactor...

    /**
     * Convert Azure Kinect Depth and Color camera co-ordinate system
     * to Unreal co-ordinate system
     * @see https://docs.microsoft.com/en-us/azure/kinect-dk/coordinate-systems
     *
     * Kinect [mm]				Unreal [cm]
     * --------------------------------------
     * +ve X-axis		Right		+ve Y-axis
     * +ve Y-axis		Down		-ve Z-axis
     * +ve Z-axis		Forward		+ve X-axis
    */
    FVector position(joint.position.xyz.z,
        joint.position.xyz.x,
        -joint.position.xyz.y);
    position *= 0.1f;

    /**
     * Convert the Orientation from Kinect co-ordinate system to Unreal co-ordinate system.
     * We negate the x, y components of the jointQuaternion since we are converting from
     * Kinect's Right Hand orientation to Unreal's Left Hand orientation.
     */
    FQuat rotation(
        -joint.orientation.wxyz.x,
        -joint.orientation.wxyz.y,
        joint.orientation.wxyz.z,
        joint.orientation.wxyz.w);

    return FTransform(rotation, position);
}


/*
 * UAzureKinectDevice::Update
 */
void UAzureKinectDevice::Update(UTextureRenderTarget2D *rt,
        TArray<uint8>&& data,
        const int32 width,
        const int32 height,
        const int32 pitch) {
    assert(rt);
    assert(data);
    assert(data.Num() == height * pitch);

    FUpdateTextureRegion2D region(0, 0, 0, 0, width, height);

    ENQUEUE_RENDER_COMMAND(UpdateRTCommand)(
        [rt, d = MoveTemp(data), region, pitch](
                FRHICommandListImmediate& cmdList) {
            auto res = rt->GetRenderTargetResource();
            if (res) {
                GDynamicRHI->RHIUpdateTexture2D(
                    cmdList,
                    res->GetRenderTargetTexture(),
                    0,
                    region,
                    pitch,
                    d.GetData());
            } else {
                UE_LOG(AzureKinectDeviceLog,
                    Warning,
                    TEXT("Failed to obtain resource from render target."));
            }
        });
}


/*
 * UAzureKinectDevice::CaptureBodyIndexTexture
 */
void UAzureKinectDevice::CaptureBodyIndexTexture(const k4abt::frame& frame) {
    const auto indexMap = frame.get_body_index_map();
    const auto width = indexMap.get_width_pixels();
    const auto height = indexMap.get_height_pixels();

    if ((width == 0) || (height == 0)) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("Body index map is empty."));
        return;
    }

    if (!HasSize(this->BodyIndexTexture, width, height)) {
        this->BodyIndexTexture->InitCustomFormat(width, height,
            EPixelFormat::PF_R8G8B8A8, true);
        this->BodyIndexTexture->RenderTargetFormat
            = ETextureRenderTargetFormat::RTF_RGBA8;
        this->BodyIndexTexture->UpdateResource();

    } else {
        auto d = indexMap.get_buffer();

        TArray<uint8> data;
        data.Reset(width * height * 4);

        for (int i = 0; i < width * height; i++) {
            data.Push(d[i]);
            data.Push(d[i]);
            data.Push(d[i]);
            data.Push(0xff);
        }

        Update(this->BodyIndexTexture,
            MoveTemp(data),
            width,
            height,
            4 * width);
    }
}


/*
 * UAzureKinectDevice::CaptureColourTexture
 */
void UAzureKinectDevice::CaptureColourTexture(k4a::capture& capture) {
    assert(capture);
    int32 width = 0;
    int32 height = 0;
    uint8 *source = nullptr;

    if (this->Remapping == EKinectRemap::COLOUR_TO_DEPTH) {
        auto colour = capture.get_color_image();
        if (!colour) {
            UE_LOG(AzureKinectDeviceLog,
                Verbose,
                TEXT("Azure Kinect colour capture is invalid."));
            return;
        }

        auto depth = capture.get_depth_image();
        if (!depth) {
            UE_LOG(AzureKinectDeviceLog,
                Verbose,
                TEXT("Azure Kinect depth capture is invalid."));
            return;
        }

        width = depth.get_width_pixels();
        height = depth.get_height_pixels();

        if ((width == 0) || (height == 0)) {
            UE_LOG(AzureKinectDeviceLog,
                Warning,
                TEXT("Azure Kinect depth image is empty."));
            return;
        }

        if (!this->_remapImage || !this->_remapImage.is_valid()) {
            this->_remapImage = k4a::image::create(
                K4A_IMAGE_FORMAT_COLOR_BGRA32,
                width,
                height,
                width * static_cast<int>(sizeof(uint8) * 4));
        }

        try {
            this->_transform.color_image_to_depth_camera(
                depth,
                colour,
                &this->_remapImage);
        } catch (k4a::error ex) {
            FString msg(ANSI_TO_TCHAR(ex.what()));
            UE_LOG(AzureKinectDeviceLog,
                Error,
                TEXT("Failed to transform colour to depth: %s"), *msg);
            return;
        }

        source = this->_remapImage.get_buffer();

    } else {
        auto colour = capture.get_color_image();
        if (!colour) {
            UE_LOG(AzureKinectDeviceLog,
                Verbose,
                TEXT("Azure Kinect colour capture is invalid."));
            return;
        }

        width = colour.get_width_pixels();
        height = colour.get_height_pixels();

        if ((width == 0) || (height == 0)) {
            UE_LOG(AzureKinectDeviceLog,
                Warning,
                TEXT("Azure Kinect colour image is empty."));
            return;
        }

        source = colour.get_buffer();
    }

    if (!HasSize(this->ColourTexture, width, height)) {
        this->ColourTexture->InitCustomFormat(width, height,
            EPixelFormat::PF_B8G8R8A8, false);
        this->ColourTexture->RenderTargetFormat
            = ETextureRenderTargetFormat::RTF_RGBA8;
        this->ColourTexture->UpdateResource();

    } else {
        Update(this->ColourTexture,
            source,
            width,
            height,
            4 * width);
    }
}


/*
 * UAzureKinectDevice::CaptureDepthTexture
 */
void UAzureKinectDevice::CaptureDepthTexture(k4a::capture& capture) {
    assert(capture);
    int32 width = 0;
    int32 height = 0;
    uint8 *source = nullptr;

    if (this->Remapping == EKinectRemap::DEPTH_TO_COLOUR) {
        auto colour = capture.get_color_image();
        if (!colour) {
            UE_LOG(AzureKinectDeviceLog,
                Verbose,
                TEXT("Azure Kinect colour capture is invalid."));
            return;
        }

        auto depth = capture.get_depth_image();
        if (!depth) {
            UE_LOG(AzureKinectDeviceLog,
                Verbose,
                TEXT("Azure Kinect depth capture is invalid."));
            return;
        }

        width = colour.get_width_pixels();
        height = colour.get_height_pixels();

        if ((width == 0) || (height == 0)) {
            UE_LOG(AzureKinectDeviceLog,
                Warning,
                TEXT("Azure Kinect depth image is empty."));
            return;
        }

        if (!this->_remapImage) {
            this->_remapImage = k4a::image::create(
                K4A_IMAGE_FORMAT_DEPTH16,
                width,
                height,
                width * static_cast<int>(sizeof(uint16)));
        }

        try {
            this->_transform.depth_image_to_color_camera(depth,
                &this->_remapImage);
        } catch (k4a::error ex) {
            FString msg(ANSI_TO_TCHAR(ex.what()));
            UE_LOG(AzureKinectDeviceLog,
                Error,
                TEXT("Failed to transform depth to colour: %s"), *msg);
            return;
        }

        source = this->_remapImage.get_buffer();

    } else {
        auto depth = capture.get_depth_image();
        if (!depth) {
            UE_LOG(AzureKinectDeviceLog,
                Verbose,
                TEXT("Azure Kinect depth capture is invalid."));
            return;
        }

        if ((width == 0) || (height == 0)) {
            UE_LOG(AzureKinectDeviceLog,
                Warning,
                TEXT("Azure Kinect depth image is empty."));
            return;
        }

        source = depth.get_buffer();
    }

    if (!HasSize(this->DepthTexture, width, height)) {
        this->DepthTexture->InitCustomFormat(width,
            height,
            EPixelFormat::PF_R8G8B8A8,
            true);
        this->DepthTexture->RenderTargetFormat
            = ETextureRenderTargetFormat::RTF_RGBA8;
        this->DepthTexture->UpdateResource();

    } else {
        TArray<uint8> data;
        data.Reset(width * height * 4);

        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                int index = h * width + w;
                uint16 r = source[index * 2];
                uint16 g = source[index * 2 + 1];
                uint16 sample = g << 8 | r;

                data.Push(source[index * 2]);
                data.Push(source[index * 2 + 1]);
                data.Push(sample > 0 ? 0x00 : 0xFF);
                data.Push(0xFF);
            }
        }

        Update(this->DepthTexture,
            MoveTemp(data),
            width,
            height,
            4 * width);
    }
}


/*
 * UAzureKinectDevice::CaptureInfraredTexture
 */
void UAzureKinectDevice::CaptureInfraredTexture(k4a::capture& capture) {
    assert(capture);
    int32 width = 0;
    int32 height = 0;

    auto image = capture.get_ir_image();
    if (!image) {
        UE_LOG(AzureKinectDeviceLog,
            Verbose,
            TEXT("Azure Kinect infrared capture is invalid."));
        return;
    }

    width = image.get_width_pixels();
    height = image.get_height_pixels();

    if ((width == 0) || (height == 0)) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("Azure Kinect infrared image is empty."));
        return;
    }

    if (!HasSize(this->InfraredTexture, width, height)) {
        this->InfraredTexture->InitCustomFormat(width, height,
            EPixelFormat::PF_R8G8B8A8, true);
        this->InfraredTexture->RenderTargetFormat
            = ETextureRenderTargetFormat::RTF_RGBA8;
        this->InfraredTexture->UpdateResource();

    } else {
        auto source = image.get_buffer();
        TArray<uint8> data;
        data.Reset(width * height * 4);

        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                int index = h * width + w;
                uint16 r = source[index * 2];
                uint16 g = source[index * 2 + 1];
                uint16 sample = g << 8 | r;

                data.Push(source[index * 2]);
                data.Push(source[index * 2 + 1]);
                data.Push(sample > 0 ? 0x00 : 0xFF);
                data.Push(0xFF);
            }
        }

        Update(this->InfraredTexture,
            MoveTemp(data),
            width,
            height,
            4 * width);
    }
}


/*
 * UAzureKinectDevice::UpdateAsync
 */
void UAzureKinectDevice::UpdateAsync(void) {
    if (!this->_device) {
        UE_LOG(AzureKinectDeviceLog,
            Warning,
            TEXT("Cannot update closed Azure Kinect."));
        return;
    }

    k4a::capture capture;

    try {
        if (!this->_device.get_capture(&capture, this->_frameTime)) {
            UE_LOG(AzureKinectDeviceLog,
                Verbose,
                TEXT("Azure Kinect capture timed out."));
        }
    } catch (k4a::error ex) {
        FString msg(ANSI_TO_TCHAR(ex.what()));
        UE_LOG(AzureKinectDeviceLog,
            Error,
            TEXT("Failed capturing from Azure Kinect: %s"), *msg);
        return;
    }

    if ((this->ColourResolution != EKinectColourResolution::RESOLUTION_OFF)
            && (this->ColourTexture != nullptr)) {
        this->CaptureColourTexture(capture);
    }

    if ((this->DepthMode != EKinectDepthMode::OFF)
            && (this->DepthTexture != nullptr)) {
        this->CaptureDepthTexture(capture);
    }

    if ((this->DepthMode != EKinectDepthMode::OFF)
            && (this->InfraredTexture != nullptr)) {
        this->CaptureInfraredTexture(capture);
    }

    if ((this->SkeletonTracking != EKinectTrackerProcessing::DISABLED)
            && this->_bodyTracker) {
        this->UpdateSkeletons(capture);
    }
}


/*
 * UAzureKinectDevice::UpdateSkeletons
 */
void UAzureKinectDevice::UpdateSkeletons(k4a::capture& capture) {
    assert(capture);
    k4abt::frame frame;
    TArray<int32> ids;

    try {
        if (!this->_bodyTracker.enqueue_capture(capture, this->_frameTime)) {
            UE_LOG(AzureKinectDeviceLog,
                Warning,
                TEXT("Failed to queue body tracking."));
            return;
        }

        if (!this->_bodyTracker.pop_result(&frame, this->_frameTime)) {
            UE_LOG(AzureKinectDeviceLog,
                Warning,
                TEXT("Failed to obtain body tracking frame in time."));
            return;
        }
    } catch (k4a::error& ex) {
        FString msg(ANSI_TO_TCHAR(ex.what()));
        UE_LOG(AzureKinectDeviceLog,
            Error,
            TEXT("Failed to obtain body tracking frame: %s"), *msg);
    }

    if (this->BodyIndexTexture) {
        this->CaptureBodyIndexTexture(frame);
    }

    FScopeLock l(&this->_lock);
    this->_cntTrackedSkeletons = frame.get_num_bodies();
    this->_skeletons.Reset(this->_cntTrackedSkeletons);

    for (int32 s = 0; s < this->_cntTrackedSkeletons; ++s) {
        k4abt_body_t body;
        FAzureKinectSkeleton skeleton;

        frame.get_body_skeleton(s, body.skeleton);
        skeleton.ID = frame.get_body_id(s);

        skeleton.Joints.Reset(K4ABT_JOINT_COUNT);
        for (int32 j = 0; j < K4ABT_JOINT_COUNT; ++j) {
            skeleton.Joints.Push(ToTransform(body.skeleton.joints[j], j));
        }

        this->_skeletons.Push(skeleton);
    }
}
