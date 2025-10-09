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

#include "AnimNode_AzureKinectPose.h"

#include "Animation/AnimInstanceProxy.h"

#include "AnimationRuntime.h"
#include "k4abttypes.h"


DEFINE_LOG_CATEGORY(AzureKinectAnimNodeLog);


/*
 * FAnimNode_AzureKinectPose::FAnimNode_AzureKinectPose
 */
FAnimNode_AzureKinectPose::FAnimNode_AzureKinectPose(void) {
    this->BonesToModify.Reserve(K4ABT_JOINT_COUNT);
    for (int i = 0; i < K4ABT_JOINT_COUNT; i++) {
        this->BonesToModify.Add(
            static_cast<EKinectBodyJoint>(i),
            FBoneReference());
    }
}


/*
 * FAnimNode_AzureKinectPose::EvaluateComponentSpace_AnyThread
 */
void FAnimNode_AzureKinectPose::EvaluateComponentSpace_AnyThread(
        FComponentSpacePoseContext& output) {
    DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateComponentSpace_AnyThread)
    output.ResetToRefPose();

    for (auto& t : this->_boneTransforms) {
        UE_LOG(AzureKinectAnimNodeLog,
            Display,
            TEXT("Update transform of bone %d."), t.BoneIndex.GetInt());
        auto xform = output.Pose.GetComponentSpaceTransform(t.BoneIndex);
        xform.SetRotation(t.Transform.Rotator().Quaternion());
        output.Pose.SetComponentSpaceTransform(t.BoneIndex, xform);

        //UE_LOG(AzureKinectAnimNodeLog,
        //    Display,
        //    TEXT("Bone %s position: %s"),
        //    *(output.AnimInstanceProxy->GetSkelMeshComponent()->GetBoneName(t.BoneIndex.GetInt()).ToString()),
        //    *(output.Pose.GetComponentSpaceTransform(t.BoneIndex).GetTranslation().ToString()));
        //UE_LOG(AzureKinectAnimNodeLog,
        //    Display,
        //    TEXT("Bone %s rotation: %s"),
        //    *(output.AnimInstanceProxy->GetSkelMeshComponent()->GetBoneName(t.BoneIndex.GetInt()).ToString()),
        //    *(output.Pose.GetComponentSpaceTransform(t.BoneIndex).Rotator().ToString()));
    }
}


/*
 * FAnimNode_AzureKinectPose::Update_AnyThread
 */
void FAnimNode_AzureKinectPose::Update_AnyThread(
        const FAnimationUpdateContext &context) {
    DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Update_AnyThread);
    this->GetEvaluateGraphExposedInputs().Execute(context);

    auto mesh = context.AnimInstanceProxy->GetSkelMeshComponent();

    this->_boneTransforms.Reset(K4ABT_JOINT_COUNT);

    for (int i = 0; i < this->Skeleton.Joints.Num(); ++i) {
        auto joint = static_cast<EKinectBodyJoint>(i);

        if (this->BonesToModify.Contains(joint)) {
            auto bone = mesh->GetBoneIndex(this->BonesToModify[joint].BoneName);
            if (bone != INDEX_NONE) {
                FCompactPoseBoneIndex bi(bone);
                this->_boneTransforms.Emplace(bi, this->Skeleton.Joints[i]);
            }
        }
    }
}
