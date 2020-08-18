#pragma once
#include "ECS/Components/CameraComponent.h"
#include "Components/TransformComponents.h"

class CameraSystem final : public SystemBase<CameraComponent, Rotation , Translation>
{
public:
    void OnCreate() override;

    float ClampAngle(float angle, float min, float max);

};


class CameraCollisionSystem final : public SystemBase<CameraComponent>
{
public:
    void OnCreate() override;

};
