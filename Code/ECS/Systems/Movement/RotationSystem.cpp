#include "RotationSystem.h"

void RotationSystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e, Rotation& rotation, ViewDirectionData& viewDir)
    {
        auto speed = 1 * GetDeltaTime();

        auto newRot = Quat::CreateRotationVDir(viewDir.Axises);
        newRot = quaternion::CreateSlerp(rotation.Value, newRot,speed);

        rotation.Value = newRot;
    });
}