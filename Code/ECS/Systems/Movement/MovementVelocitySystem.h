#pragma once

#include "ECS/Components/MovementComponents.h"
#include "Systems/SystemBase.h"

class MovementVelocitySystem final :public SystemBase<MoveDirectionData,MovementSpeed, Velocity>
{
public:
    void OnCreate() override;
};
