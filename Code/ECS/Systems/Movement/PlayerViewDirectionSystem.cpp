#include "PlayerViewDirectionSystem.h"

void PlayerViewDirectionSystem::OnCreate()
{
    SystemRun->signature("PlayerTag").each([this](flecs::entity, ViewDirectionData& viewDirection)
    {
        const auto s = flecs::entity(*m_pWorld->DefaultWorld, "Camera");
        auto* camLocalToWorld = s.get<WorldToLocal>();

        if (camLocalToWorld ==nullptr) return;

        viewDirection.Axises = camLocalToWorld->Forward();

    });
}