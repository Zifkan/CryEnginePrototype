#include "MovementCharacterSystem.h"

void MovementCharacterSystem::OnCreate()
{
    SystemRun->each([](flecs::entity e,MovementVelocity& velocity , CharacterComponent& character, Rotation& rotation)
    {

        character.pCharacterController->GetEntity()->SetRotation(Quat(1, 0, 0, rotation.Value.v.z));
        character.pCharacterController->AddVelocity(velocity.Value);

       
       
    });
}
