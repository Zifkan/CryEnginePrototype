#pragma once
#include "ECS/Components/MovementComponents.h"
#include "Systems/SystemBase.h"

class PlayerViewDirectionSystem  final : public SystemBase<ViewDirectionData>
{
public:
    void OnCreate() override;
};
