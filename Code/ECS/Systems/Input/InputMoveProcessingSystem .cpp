#include "StdAfx.h"
#include "InputMoveProcessingSystem.h"

void InputMoveProcessingSystem::OnCreate()
{
    SystemRun->each([](flecs::entity e, InputComponent& input)
    {
        CryLog("Action: x = %f; y = %f", input.MovementAxis.x, input.MovementAxis.y);
    });
}