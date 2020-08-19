#include "CharacterRotationSystem.h"

void CharacterRotationSystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e, Rotation& rotation, ViewDirectionData& viewDir)
    {
        auto newRot = Quat::CreateRotationVDir(viewDir.Axises.normalize());     
        rotation.Value = newRot;
    });
}