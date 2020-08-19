#include "MovementCharacterSystem.h"

void MovementCharacterSystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e,MovementVelocity& velocity , CharacterComponent& character, Rotation& rotation)
    {
        Quat rot = ZERO;

        if (velocity.Value.len() > 0)
        {
            if ((lastRot - rotation.Value).v.len() > 0.2f)
            {
                rot = Quat(rotation.Value.w, 0, 0, rotation.Value.v.z);
                character.pCharacterController->GetEntity()->SetRotation(rot);
            }
            else
            {
                rot = Quat::CreateRotationVDir(Vec3(velocity.Value.x, velocity.Value.y, 0).normalize());
                character.pCharacterController->GetEntity()->SetRotation(rot);
            }
        }

        character.pCharacterController->AddVelocity(velocity.Value);
             
        lastRot = rotation.Value;
    });
}
