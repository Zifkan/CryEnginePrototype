#include "StdAfx.h"
#include "InputMoveProcessingSystem.h"
#include "ECS/Components/InputComponent.h"


void InputMoveProcessingSystem::OnCreate()
{
    SystemRun->each([this](flecs::entity e, PlayerTag& player, MoveDirectionData& moveDirection, ViewDirectionData& viewDirection)
    {
        const auto s = flecs::entity(*m_pWorld->DefaultWorld, flecs::Singleton);
        const InputComponent* input = s.get<InputComponent>();

        if (input==nullptr) return;

        Vec3 dir = viewDirection.Axises;
        dir.z = 0;

        const Vec3 moveDir = input->MovementAxis.y * dir + input->MovementAxis.x * dir.cross(Vec3(0,0,1));    
        moveDirection.Axises = moveDir;
        CryLogAlways("Velocity: x = %f, y = %f, z = %f", moveDir.x, moveDir.y, moveDir.z);
    });
}