#pragma once
#include "ECS/Components/InputComponent.h"
#include "Systems/SystemBase.h"

class InputMoveProcessingSystem final :public SystemBase<InputComponent>
{
public:
     void OnCreate() override;
};
