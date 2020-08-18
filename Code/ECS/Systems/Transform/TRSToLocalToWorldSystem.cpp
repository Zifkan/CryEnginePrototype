#include "TRSToLocalToWorldSystem.h"

void TRSToLocalToWorldSystem::OnCreate()
{
    SystemRun->each([](flecs::entity e,LocalToWorld& localToWorld, Rotation& rot, Translation& translation)
    {
        localToWorld.Value.Set(Vec3(1),rot.Value,translation.Value);
    });
}
