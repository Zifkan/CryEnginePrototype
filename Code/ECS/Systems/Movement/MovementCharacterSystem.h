#pragma once

#include "Components/TransformComponents.h"
#include "ECS/Components/CharacterComponents.h"
#include "ECS/Components/MovementComponents.h"
#include "Systems/SystemBase.h"

class MovementCharacterSystem final : public SystemBase<MovementVelocity, CharacterComponent, Rotation>
{
public:
    void OnCreate() override;
};
