#pragma once
#include "ECS/Components/InputComponent.h"

class InputFireProcessingSystem  final : public SystemBase<InputComponent>
{
public:
    void OnCreate() override;
};
