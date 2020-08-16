#include "MovementVelocitySystem.h"

void MovementVelocitySystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e, MoveDirectionData& moveData, MovementSpeed& movementSpeed, Velocity& velocity)
    {
        auto vel = movementSpeed.Speed *  GetDeltaTime() * moveData.Axises;
      
        velocity.Value = vel;
    });
}
