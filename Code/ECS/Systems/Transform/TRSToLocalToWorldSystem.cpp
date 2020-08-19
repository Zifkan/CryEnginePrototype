#include "TRSToLocalToWorldSystem.h"

void TRSToLocalToWorldSystem::OnCreate()
{
    SystemRun->each([](flecs::entity e,LocalToWorld& localToWorld, Rotation& rot, Translation& translation)
    {
        const auto scaleComponent = e.get<Scale>();
        auto scale = Vec3(1);
        if(scaleComponent!=nullptr)
        {
            scale = scaleComponent->Value;
        }

        localToWorld.Value.Set(scale,rot.Value,translation.Value);
    });
}
