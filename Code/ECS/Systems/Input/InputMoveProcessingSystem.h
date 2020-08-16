#pragma once
#include "ECS/Components/MovementComponents.h"
#include "ECS/Components/CharacterComponents.h"
#include "Systems/SystemBase.h"

class InputMoveProcessingSystem final :public SystemBase<PlayerTag,MoveDirectionData, ViewDirectionData>
{
public:
     void OnCreate() override;
    
};
