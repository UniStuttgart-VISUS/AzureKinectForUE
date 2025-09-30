// <copyright file="AnimNode_AzureKinectPose.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "AnimGraphNode_AzureKinectPose.h"

#include "AnimNodes/AnimGraphNode_AzureKinectPose.h"

#include "AnimationGraphSchema.h"


#define LOCTEXT_NAMESPACE "AzureKinectPose"


/*
 * UAnimGraphNode_AzureKinectPose::CreateOutputPins
 */
void UAnimGraphNode_AzureKinectPose::CreateOutputPins(void) {
    this->CreatePin(EGPD_Output,
        UAnimationGraphSchema::PC_Struct,
        FComponentSpacePoseLink::StaticStruct(),
        TEXT("ComponentPose"));
}


/*
 * UAnimGraphNode_AzureKinectPose::GetNodeCategory
 */
FString UAnimGraphNode_AzureKinectPose::GetNodeCategory(void) const {
    return FString("Azure Kinect");
}


/*
 * UAnimGraphNode_AzureKinectPose::GetNodeTitle
 */
FText UAnimGraphNode_AzureKinectPose::GetNodeTitle(
        ENodeTitleType::Type TitleType) const {
    return LOCTEXT("AzureKinectPose", "Azure Kinect Pose");
}


/*
 * UAnimGraphNode_AzureKinectPose::GetNodeTitleColor
 */
FLinearColor UAnimGraphNode_AzureKinectPose::GetNodeTitleColor(void) const {
    return FLinearColor(0.f, 0.f, 0.f);
}


/*
 * UAnimGraphNode_AzureKinectPose::GetTooltipText
 */
FText UAnimGraphNode_AzureKinectPose::GetTooltipText(void) const {
    return LOCTEXT(
        "AnimGraphNode_AzureKinectPose_Tooltip",
        "Process AzureKinect skeleton input into pose"
    );
}

#undef LOCTEXT_NAMESPACE
