// <copyright file="AnimNode_AzureKinectPose.h" company="Visualisierungsinstitut der Universität Stuttgart">
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

#include "AnimGraphNode_Base.h"
#include "AnimNodeEditModes.h"
#include "AnimNode_AzureKinectPose.h"
#include "CoreMinimal.h"

#include "EdGraph/EdGraphNodeUtils.h"

#include "AnimGraphNode_AzureKinectPose.generated.h"


UCLASS(meta = (Kerwords = "Azure Kinect"))
class UAnimGraphNode_AzureKinectPose : public UAnimGraphNode_Base {
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, Category = "Settings")
    FAnimNode_AzureKinectPose Node;

    virtual void CreateOutputPins(void) override;

    virtual FString GetNodeCategory(void) const override;

    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

    virtual FLinearColor GetNodeTitleColor(void) const override;

    virtual FText GetTooltipText(void) const override;
};
