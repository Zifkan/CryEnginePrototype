#include "CopyTransformFromCryEntitySystem.h"

void CopyTransformFromCryEntitySystem::OnCreate()
{
    SystemRun->each([](flecs::entity e, CopyTransformFromCryEntity& v, LocalToWorld& localToWorld, CryEntityComponent& cryEntity)
    {
        localToWorld.Value = cryEntity.Value->GetWorldTM();
    });
}
