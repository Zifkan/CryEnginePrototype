#include "RotationSystem.h"

void CharacterRotationSystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e, Rotation& rotation, ViewDirectionData& viewDir)
    {
      //  auto speed = 10 * GetDeltaTime();

        auto newRot = Quat::CreateRotationVDir(viewDir.Axises.normalize());
       // newRot = quaternion::CreateSlerp(rotation.Value, newRot,speed);

        rotation.Value = newRot.Normalize();
    });
}