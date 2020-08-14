#pragma once
#include "ECS/Components/CameraComponent.h"

class CameraSystem final : public SystemBase<CameraComponent>
{
public:
    void OnCreate() override;

    float ClampAngle(float angle, float min, float max);

private:
    float x;
    float y;
};
