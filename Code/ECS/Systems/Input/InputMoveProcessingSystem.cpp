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

        Vec3 dir = viewDirection.Axises.normalized();
        dir.z = 0;

        const Vec3 moveDir = input->MovementAxis.y * dir + input->MovementAxis.x * Vec3(dir.y, -dir.x, dir.z);
       
        moveDirection.Axises = moveDir.dot(moveDir) > 1 ? moveDir.normalized() : moveDir;
        moveDirection.Axises.z = 0;
    });
}