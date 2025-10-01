// <copyright file="AzureKinectDevice.h" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "CoreMinimal.h"

#include "Animation/SkeletalMeshActor.h"

#include "Engine/TextureRenderTarget2D.h"

#include "k4a/k4a.hpp"

#include "k4abt.hpp"
#include "AzureKinectEnum.h"
#include "AzureKinectSkeleton.h"

#include "AzureKinectDevice.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(AzureKinectDeviceLog, Log, All);


// Forward declarations.
class FAzureKinectDeviceThread;


/// <summary>
/// Represents the Azure Kinect camera.
/// </summary>
UCLASS(BlueprintType, hidecategories = (Object))
class UNREALAZUREKINECT_API UAzureKinectDevice : public UObject {
    GENERATED_BODY()

public:

    /// <summary>
    /// Counts the number of connected Kinects.
    /// </summary>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable, Category = "Device")
    static int32 CountDevices();

    /// <summary>
    /// Specifies the acceptable default colour image format.
    /// </summary>
    static constexpr k4a_image_format_t ColourFormat
        = k4a_image_format_t::K4A_IMAGE_FORMAT_COLOR_BGRA32;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    UAzureKinectDevice();

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="initialiser"></param>
    UAzureKinectDevice(const FObjectInitializer& initialiser);

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "I/O")
    UTextureRenderTarget2D *BodyIndexTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Device settings")
    EKinectColourResolution ColourResolution;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "I/O")
    UTextureRenderTarget2D *ColourTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Device settings")
    EKinectDepthMode DepthMode;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "I/O")
    UTextureRenderTarget2D *DepthTexture;

    UPROPERTY(BlueprintReadWrite, Category = "Device")
    int32 DeviceIndex;

    TArray<TSharedPtr<FString>> Devices;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Device settings")
    bool DisableStreamingIndicator;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Device settings")
    EKinectFps FrameRate;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "I/O")
    UTextureRenderTarget2D *InfraredTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Device settings")
    EKinectRemap Remapping;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Device settings")
    EKinectSensorOrientation SensorOrientation;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Device settings")
    EKinectTrackerProcessing SkeletonTracking;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Device settings")
    bool SynchronisedImagesOnly;

    /// <summary>
    /// Gets the skeleton at the give zero-based index.
    /// </summary>
    /// <param name="index"></param>
    /// <returns>The skeleton at the specified index or a
    /// null-skeleton if the index is out of range.</returns>
    UFUNCTION(BlueprintCallable, Category = "Skeletons")
    FAzureKinectSkeleton GetSkeleton(const int32 index) const;

    /// <summary>
    /// Returns a snapshot of the currently tracked skeletons.
    /// </summary>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable, Category = "Skeletons")
    TArray<FAzureKinectSkeleton> GetSkeletons() const;

    /// <summary>
    /// Answer the number of currently tracked skeletons.
    /// </summary>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable, Category = "Skeletons")
    int32 GetTrackedSkeletons() const;

    /// <summary>
    /// Answer whether there is any tracked skeleton
    /// </summary>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable, Category = "Skeletons")
    inline bool HasTrackedSkeletons() const {
        return (this->GetTrackedSkeletons() > 0);
    }

    /// <summary>
    /// Answer whether the device is open.
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Device")
    inline bool IsOpen() const noexcept {
        return static_cast<bool>(this->_device);
    }

    /// <summary>
    /// Refreshes the list of connected devices.
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Device")
    int32 RefreshDevices();

    /// <summary>
    /// Opens the selected device and starts the camera.
    /// </summary>
    /// <returns><see langword="true"/> on success, <see langword="false"/>
    /// in case of an error.</returns>
    UFUNCTION(BlueprintCallable, Category = "Device")
    bool Start();

    /// <summary>
    /// Stops the camera and closes the device.
    /// </summary>
    /// <returns></returns>
    UFUNCTION(BlueprintCallable, Category = "Device")
    bool Stop();

private:

    static FString GetPluginLocation(void);

    static inline bool HasSize(const UTextureRenderTarget2D *texture,
            const int32 width,
            const int32 height) noexcept {
        return (texture != nullptr)
            && (texture->GetSurfaceWidth() == width)
            && (texture->GetSurfaceHeight() == height);
    }

    static std::chrono::milliseconds ToFrameTime(
        const EKinectFps frameRate) noexcept;

    static FTransform ToTransform(const k4abt_joint_t& joint,
        const int32 index);

    static void Update(UTextureRenderTarget2D *rt,
        TArray<uint8>&& data,
        const int32 width,
        const int32 height,
        const int32 pitch);

    static inline void Update(UTextureRenderTarget2D *rt,
            const void *data,
            const int32 width,
            const int32 height,
            const int32 pitch) {
        TArray<uint8> d(static_cast<const uint8 *>(data), height * pitch);
        UAzureKinectDevice::Update(rt, std::move(d), width, height, pitch);
    }

    void CaptureBodyIndexTexture(const k4abt::frame& frame);

    void CaptureColourTexture(k4a::capture& capture);

    void CaptureDepthTexture(k4a::capture& capture);

    void CaptureInfraredTexture(k4a::capture& capture);

    /// <summary>
    /// This method is called periodically be the
    /// <see cref="FAzureKinectDeviceThread"/>.
    /// </summary>
    void UpdateAsync(void);

    void UpdateSkeletons(k4a::capture& capture);

    k4abt::tracker _bodyTracker;
    k4a::calibration _calibration;
    int32 _cntTrackedSkeletons;
    k4a::device _device;
    std::chrono::milliseconds _frameTime;
    mutable FCriticalSection _lock;
    k4a::image _remapImage;
    TArray<FAzureKinectSkeleton> _skeletons;
    k4a::transformation _transform;
    FAzureKinectDeviceThread *_thread;

    friend class FAzureKinectDeviceThread;
};
