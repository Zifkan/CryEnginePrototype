#include "MovementCharacterSystem.h"

void MovementCharacterSystem::OnCreate()
{
    SystemRun->each([](flecs::entity e,Velocity& velocity , CharacterComponent& character, Rotation& rotation)
    {
        character.pCharacterController->AddVelocity(velocity.Value);

       
        character.pCharacterController->GetEntity()->SetRotation(Quat( rotation.Value.w,0,0, rotation.Value.v.z));
    });
}
