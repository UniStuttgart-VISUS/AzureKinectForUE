// <copyright file="AzureKindexEnum.h" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "AzureKinectEnum.generated.h"


// These enumerations are the Blueprint equivalents of the enums defined in the
// Azure Kinect SDK. The numeric values must be identical to the SDK values.


UENUM(BlueprintType, Category = "Azure Kinect|Enums")
enum class EKinectDepthMode : uint8 {
    /** Depth sensor will be turned off with this setting. */
    OFF = 0         UMETA(DisplayName = "Depth mode off"),

    /** Depth captured at 320x288. Passive IR is also captured at 320x288. */
    NFOV_2X2BINNED  UMETA(DisplayName = "NFOV 2x2 binned (320x288)"),

    /** Depth captured at 640x576. Passive IR is also captured at 640x576. */
    NFOV_UNBINNED   UMETA(DisplayName = "NFOV unbinned (640x576)"),

    /** Depth captured at 512x512. Passive IR is also captured at 512x512. */
    WFOV_2X2BINNED  UMETA(DisplayName = "WFOV 2x2 binned (512x512)"),

    /** Depth captured at 1024x1024. Passive IR is also captured at 1024x1024. */
    WFOV_UNBINNED   UMETA(DisplayName = "WFOV unbinned (1024x1024)"),

    /**< Passive IR only, captured at 1024x1024. */
    PASSIVE_IR      UMETA(DisplayName = "Passive IR (1024x1024)"),
};


UENUM(BlueprintType, Category = "Azure Kinect|Enums")
enum class EKinectColourResolution : uint8 {
    /** Colour sensor will be turned off with this setting. */
    RESOLUTION_OFF = 0  UMETA(DisplayName = "Colour camera off"),

    /** Colour captured at 1280 x 720. */
    RESOLUTION_720P     UMETA(DisplayName = "1280 x 720  [16:9]"),

    /** Colour captured at 2560 x 1440. */
    RESOLUTION_1440P    UMETA(DisplayName = "2560 x 1440 [16:9]"),

    /** Colour captured at 2048 x 1536. */
    RESOLUTION_1536P    UMETA(DisplayName = "2048 x 1536 [4:3]"),

    /** Colour captured at 3840 x 2160. */
    RESOLUTION_2160P    UMETA(DisplayName = "3840 x 2160 [16:9]"),

    /** Colour captured at 4096 x 3072. */
    RESOLUTION_3072P    UMETA(DisplayName = "4096 x 3072 [4:3]"),
};


UENUM(BlueprintType, Category = "Azure Kinect|Enums")
enum class EKinectFps : uint8 {
    PER_SECOND_5 = 0    UMETA(DisplayName = "5 fps"),
    PER_SECOND_15       UMETA(DisplayName = "15 fps"),
    PER_SECOND_30       UMETA(DisplayName = "30 fps"),
};


UENUM(BlueprintType, Category = "Azure Kinect|Enums")
enum class EKinectRemap : uint8 {
    COLOUR_TO_DEPTH = 0     UMETA(DisplayName = "Colour to Depth"),
    DEPTH_TO_COLOUR         UMETA(DisplayName = "Depth to Colour"),
};


UENUM(BlueprintType, Category = "Azure Kinect|Enums")
enum class EKinectBodyJoint : uint8 {
    PELVIS = 0      UMETA(DisplayName = "Pelvis"),
    SPINE_NAVEL     UMETA(DisplayName = "Spine Navel"),
    SPINE_CHEST     UMETA(DisplayName = "Spine Chest"),
    NECK            UMETA(DisplayName = "Neck"),
    CLAVICLE_LEFT   UMETA(DisplayName = "Clavicle Left"),
    SHOULDER_LEFT   UMETA(DisplayName = "Shoulder Left"),
    ELBOW_LEFT      UMETA(DisplayName = "Elbow Left"),
    WRIST_LEFT      UMETA(DisplayName = "Wrist Left"),
    HAND_LEFT       UMETA(DisplayName = "Hand Left"),
    HANDTIP_LEFT    UMETA(DisplayName = "Hand Tip Left"),
    THUMB_LEFT      UMETA(DisplayName = "Thumb Left"),
    CLAVICLE_RIGHT  UMETA(DisplayName = "Clavicle Right"),
    SHOULDER_RIGHT  UMETA(DisplayName = "Shoulder Right"),
    ELBOW_RIGHT     UMETA(DisplayName = "Elbow Right"),
    WRIST_RIGHT     UMETA(DisplayName = "Wrist Right"),
    HAND_RIGHT      UMETA(DisplayName = "Hand Right"),
    HANDTIP_RIGHT   UMETA(DisplayName = "Hand Tip Right"),
    THUMB_RIGHT     UMETA(DisplayName = "Thumb Right"),
    HIP_LEFT        UMETA(DisplayName = "Hip Left"),
    KNEE_LEFT       UMETA(DisplayName = "Knee Left"),
    ANKLE_LEFT      UMETA(DisplayName = "Ankle Left"),
    FOOT_LEFT       UMETA(DisplayName = "Foot Left"),
    HIP_RIGHT       UMETA(DisplayName = "Hip Right"),
    KNEE_RIGHT      UMETA(DisplayName = "Knee Right"),
    ANKLE_RIGHT     UMETA(DisplayName = "Ankle Right"),
    FOOT_RIGHT      UMETA(DisplayName = "Foot Right"),
    HEAD            UMETA(DisplayName = "Head"),
    NOSE            UMETA(DisplayName = "Nose"),
    EYE_LEFT        UMETA(DisplayName = "Eye Left"),
    EAR_LEFT        UMETA(DisplayName = "Ear Left"),
    EYE_RIGHT       UMETA(DisplayName = "Eye Right"),
    EAR_RIGHT       UMETA(DisplayName = "Ear Right"),
    COUNT           UMETA(DisplayName = "COUNT", Hidden),
};


UENUM(BlueprintType, Category = "Azure Kinect|Enums")
enum class EKinectSensorOrientation : uint8 {
    /** Mount the sensor at its default orientation */
    DEFAULT = 0         UMETA(DisplayName = "Default"),

    /** Clockwisely rotate the sensor 90 degree */
    CLOCKWISE90         UMETA(DisplayName = "Clockwise 90"),

    /**< Counter-clockwisely rotate the sensor 90 degrees */
    ANTICLOCKWISE90     UMETA(DisplayName = "Anti-clockwise 90"),

    /** Mount the sensor upside-down */
    FLIP180             UMETA(DisplayName = "Flip 180"), 
};
