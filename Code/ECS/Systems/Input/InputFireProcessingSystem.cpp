#include "StdAfx.h"
#include "InputFireProcessingSystem.h"

void InputFireProcessingSystem::OnCreate()
{
    SystemRun->each([](flecs::entity e, InputComponent& input)
    {
        
    });
}
