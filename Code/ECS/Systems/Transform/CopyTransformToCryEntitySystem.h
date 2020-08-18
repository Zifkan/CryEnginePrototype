#pragma once

#include "Components/CryEntityComponent.h"
#include "Components/TransformComponents.h"
#include "Systems/SystemBase.h"

class CopyTransformToCryEntitySystem final :public SystemBase<LocalToWorld, CryEntityComponent>
{
public:
    void OnCreate() override;
};