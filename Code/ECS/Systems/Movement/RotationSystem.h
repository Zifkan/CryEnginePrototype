#pragma once
#include "Components/TransformComponents.h"
#include "ECS/Components/MovementComponents.h"
#include "Systems/SystemBase.h"

class RotationSystem :public SystemBase<Rotation, ViewDirectionData>
{
public:
    void OnCreate() override;
};
