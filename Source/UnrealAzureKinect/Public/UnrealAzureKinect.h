// <copyright file="UnrealAzureKinect.cpp" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2025 Visualisierungsinstitut der Universit�t Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#include "Modules/ModuleManager.h"


class FUnrealAzureKinectModule : public IModuleInterface {

public:

    virtual void ShutdownModule(void) override;

    virtual void StartupModule(void) override;
};
